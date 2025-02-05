#include "lin_lib.h"
#include "can_lib.h"


// Instantiate the SerialLIN object
HardwareSerial SerialLIN(1);  // Define the SerialLIN object here

void uartSetup() {
    SerialLIN.begin(19200, SERIAL_8N1, LIN_RX_PIN, LIN_TX_PIN);
}

// Mappings
const char *getShifterName(byte id) {
    switch (id) {
    case 0x01: return "Shift -";
    case 0x02: return "Shift +";
    case 0x03: return "Shift +/-";
    default: return "Unknown";
    }
}

const char *getButtonName(byte id) {
    switch (id) {
    case 0x02: return "Src+";
    case 0x03: return "Src-";
    case 0x04: return "Arrow Up";
    case 0x05: return "Arrow Down";
    case 0x07: return "OK";
    case 0x10: return "Volume+";
    case 0x11: return "Volume-";
    case 0x15: return "Next";
    case 0x16: return "Previous";
    case 0x19: return "Voice";
    case 0x23: return "View";
    default: return "Unknown";
    }
}

const char *getAccButtonName(byte id) {
    switch (id) {
    case 0x81: return "ACC Set";
    case 0x82: return "ACC Speed +";
    case 0x84: return "ACC Speed -";
    case 0xB0: return "ACC Distance";
    case 0x88: return "ACC Resume";
    case 0x80: return "ACC Mode (Ignored)";
    default: return "Unknown ACC Button";
    }
}

const char *getAccStateName(byte id) {
    switch (id) {
    case 0x2A: return "ACC On/Off";
    case 0x2B: return "ACC Idle";
    default: return "Unknown ACC State";
    }
}


void sendBreakSignal() {
    // Manually control the TX pin for the break signal
    pinMode(LIN_TX_PIN, OUTPUT);
    digitalWrite(LIN_TX_PIN, LOW);  // Break signal (dominant)
    delayMicroseconds(1000);  // More than 13 bit lengths at 19200 baud
    pinMode(LIN_TX_PIN, INPUT_PULLUP);  // Return control to UART
    SerialLIN.begin(19200, SERIAL_8N1, LIN_RX_PIN, LIN_TX_PIN);  // Restart UART
}

byte calculateParity(byte id) {
    byte p0 = ((id >> 0) ^ (id >> 1) ^ (id >> 2) ^ (id >> 4)) & 0x01;
    byte p1 = (~((id >> 1) ^ (id >> 3) ^ (id >> 4) ^ (id >> 5))) & 0x01;
    return (id & 0x3F) | (p0 << 6) | (p1 << 7);
}

byte calculateEnhancedChecksum(byte pid, byte *data, int length) {
    unsigned int sum = pid;
    for (int i = 0; i < length; i++) {
        sum += data[i];
    }
    while (sum > 0xFF) {
        sum = (sum & 0xFF) + (sum >> 8);
    }
    return ~((byte)sum);
}

float readVoltage() {
    int rawADC = analogRead(BACKLIGHT_PIN);  
    float voltage = (rawADC / 4095.0) * 3.3;  // Convert ADC value to voltage (for 3.3V reference)
    return voltage;
}

int calculateBacklight(float voltage) {
    Serial.print("Voltage: ");
    Serial.print(String(voltage));
    Serial.print(" -> Brightness HEX: ");

    int brightness;

    if (voltage < 0.5) {
        brightness = 0x00;  // Off
    } else if (voltage >= 2.1) {
        brightness = 0x64;  // Max brightness at 2.0V
    } else {
        // Linear Mapping: 0.5V -> 0x08, 2.0V -> 0x64
        brightness = round(((voltage - 0.5) / (2.1 - 0.5)) * (0x64 - 0x08) + 0x08);
    }

    return brightness;
}

void sendIgnitionFrame() {
    // Step 1: Read the voltage and calculate the backlight value
    float voltage = readVoltage();  // Function to read voltage from GPIO 13
    byte backlight = calculateBacklight(voltage);  // Function to map voltage to backlight value

    // Step 2: Prepare LIN frame data
    byte rawId = 0x0D;  // LIN frame ID for ignition
    byte pid = calculateParity(rawId);  // Calculate parity for the frame ID
    byte data[] = {backlight, 0xFF, 0xFF, 0xFF};  // Data with dynamic backlight value
    byte checksum = calculateEnhancedChecksum(pid, data, sizeof(data));  // Calculate checksum

    // Step 3: Send the LIN frame
    sendBreakSignal();  // Send break signal
    SerialLIN.write(0x55);  // Sync byte
    SerialLIN.write(pid);  // Protected ID (PID)

    // Send data bytes
    for (int i = 0; i < sizeof(data); i++) {
        SerialLIN.write(data[i]);
    }

    // Send checksum
    SerialLIN.write(checksum);
    SerialLIN.flush();  // Ensure all data is transmitted before continuing
}

void sendButtonRequestFrame() {
    byte rawId = 0x8E;
    byte pid = calculateParity(rawId);

    sendBreakSignal();
    SerialLIN.write(0x55);
    SerialLIN.write(pid);
    SerialLIN.flush();  // Ensure all data is transmitted before continuing
}

void sendAccRequestFrame() {
    byte rawId = 0x0F;
    byte pid = calculateParity(rawId);

    sendBreakSignal();
    SerialLIN.write(0x55);
    SerialLIN.write(pid);
    SerialLIN.flush();  // Ensure all data is transmitted before continuing
}

void pressMFAButton(int buttonPin) {
    digitalWrite(buttonPin, HIGH);  // Press button (simulate short to ground)
    delay(100);  // Hold button for 100ms 
    digitalWrite(buttonPin, LOW);  // Release button
}

void listenForResponse(byte *response, int &index) {
    unsigned long startTime = millis();
    bool frameStarted = false;
    index = 0;

    while (millis() - startTime < LIN_TIMEOUT) {
        if (SerialLIN.available()) {
            byte incomingByte = SerialLIN.read();

            // Start of new frame detection
            if (incomingByte == 0x55) {
                if (frameStarted) {
                    // Process the previous frame before starting a new one
                    parseResponse(response, index);
                    index = 0;  // Reset index for new frame
                }
                frameStarted = true;  // Frame starts
            }

            if (frameStarted) {
                response[index++] = incomingByte;  // Store the byte
                if (index >= BUFFER_SIZE) {
                    // Prevent buffer overflow
                    parseResponse(response, index);
                    index = 0;
                    frameStarted = false;
                }
            }
        }
    }

    if (frameStarted && index > 0) {
        parseResponse(response, index);
    }
}

void parseResponse(byte *response, int length) {
  static unsigned long lastTransmitTime = 0;
    unsigned long currentTime = millis();

    if (currentTime - lastTransmitTime < 1000) {  // Ensure at least 1 second between transmissions
        return;  // Skip this transmission
    }

    if (response[1] == 0x8E) {
        if (response[3] != 0) {
          //translateToCan(response);
            Serial.print("Button 1: ");
            Serial.print(getButtonName(response[3]));

            if (response[3] == 0x04) {
                pressMFAButton(MFA_UP_PIN);  // Simulate MFA UP button
            } else if (response[3] == 0x05) {
                pressMFAButton(MFA_DOWN_PIN);  // Simulate MFA DOWN button
            } else if (response[3] == 0x07) {
                pressMFAButton(MFA_RESET_PIN);  // Simulate MFA RESET button
            }

            if (response[4] != 0) {
                Serial.print(" + Button 2: ");
                Serial.print(getButtonName(response[4]));
            }
            Serial.print(" Status: ");
            Serial.print(String(response[5], HEX));
            Serial.println();
        }
        if (response[7] != 0) {
            Serial.print("Shifter 7: ");
            for (int i = 0; i < 10; i++) {
                Serial.print(" ");
                Serial.print(String(response[i], HEX));
            }
            Serial.println();
        }
        if (response[8] != 0) {
            Serial.print("Shifter 8: ");
            Serial.println(getShifterName(response[8]));
        }
        if (response[9] != 0) {
            Serial.print("Horn: ");
            Serial.println(String(response[9] == 0x01, HEX));
        }
    } else if (response[1] == 0xCF) {
        if (response[4] != 0x80) {
            Serial.print("ACC Button: ");
            Serial.println(getAccButtonName(response[4]));
        }
        if (response[5] != 0x2B) {
            Serial.print("ACC State: ");
            Serial.println(getAccStateName(response[5]));
        }
    }
}
