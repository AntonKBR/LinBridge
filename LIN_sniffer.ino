#define LIN_TX_PIN 1  // TX1 pin on Arduino Mega (Hardware Serial1)

const char* keyMapping[] = {
  "None",         // 0x00
  "Unknown",      // 0x01
  "Src+",         // 0x02
  "Src-",         // 0x03
  "Arrow Up",     // 0x04
  "Arrow Down",   // 0x05
  "Unknown",      // 0x06
  "OK",           // 0x07
  "Unknown",      // 0x08
  "Unknown",      // 0x09
  "Volume+",      // 0x0A
  "Unknown",      // 0x0B
  "Unknown",      // 0x0C
  "Unknown",      // 0x0D
  "Unknown",      // 0x0E
  "Unknown",      // 0x0F
  "Volume+",      // 0x10
  "Volume-",      // 0x11
  "Unknown",      // 0x12
  "Unknown",      // 0x13
  "Unknown",      // 0x14
  "Next",         // 0x15
  "Previous",     // 0x16
  "Unknown",      // 0x17
  "Unknown",      // 0x18
  "Voice",        // 0x19
  "Unknown",      // 0x1A
  "Unknown",      // 0x1B
  "Unknown",      // 0x1C
  "Unknown",      // 0x1D
  "Unknown",      // 0x1E
  "Unknown",      // 0x1F
  "Unknown",      // 0x20
  "Unknown",      // 0x21
  "Unknown",      // 0x22
  "View",         // 0x23
};

const char* shifterMapping[] = {
  "empty",
  "Shift -",      //1
  "Shift +",      //2
  "Shift + -",     //3
};


void setup() {
  Serial.begin(115200);                 // Debugging output
  Serial1.begin(19200, SERIAL_8N1);     // LIN communication at 19,200 baud
  Serial.println("LIN Master Initialized");
}

void loop() {
  sendLINFrame();                       // Send the 0x0D LIN frame (ignition signal)
  sendFrame8E();                        // Send the 0x8E LIN header only
  listenForResponse();                  // Capture and print responses if button pressed
}

void sendBreakSignal() {
  Serial1.end();
  Serial1.begin(9600, SERIAL_8N1);      // Temporarily lower baud rate to generate a break
  Serial1.write(0x00);                  // Send a null byte (dominant signal)
  Serial1.end();
  Serial1.begin(19200, SERIAL_8N1);     // Restore LIN baud rate
}

void sendLINFrame() {
  byte rawId = 0x0D;                    // Raw ID (6 bits) for the first frame
  byte pid = calculateParity(rawId);    // Calculate PID with parity bits
  byte data[] = {0x64, 0x81, 0xFF, 0xFF};  // Data bytes for the first frame

  byte checksum = calculateEnhancedChecksum(pid, data, sizeof(data));  // Enhanced checksum

  sendBreakSignal();    
  Serial1.write(0x55);                  // Sync Byte
  Serial1.write(pid);                   // PID

  for (int i = 0; i < sizeof(data); i++) {
    Serial1.write(data[i]);             // Data bytes
  }

  Serial1.write(checksum);              // Checksum
}

void sendFrame8E() {
  byte rawId = 0x8E;                    // Raw ID (6 bits) for the second frame
  byte pid = calculateParity(rawId);    // Calculate PID with parity bits

  sendBreakSignal();
  Serial1.write(0x55);                  // Sync Byte
  Serial1.write(pid);                   // PID only (no data or checksum)
}

void listenForResponse() {
  unsigned long startTime = millis();
  byte response[16] = {0};             // Buffer for up to 16 bytes
  int index = 0;

  while (millis() - startTime < 75) {   // Timeout of 75ms
    if (Serial1.available()) {
      response[index++] = Serial1.read();
      if (index >= sizeof(response)) break;  // Stop if the buffer is full
    }
  }

  if (index > 0) {
    // Check if any button-related data is non-zero
    if (response[3] != 0 || response[4] != 0 || response[5] != 0 || response[7] != 0 || response[8] != 0 || response[9] != 0) {
      // Interpret and display button presses
      if (response[3] != 0) {
        Serial.print(getButtonName(response[3]));
        Serial.print(" (ID: ");
        Serial.print(response[3], HEX);
        Serial.print(") ");
      }
      if (response[4] != 0) {
        Serial.print(getButtonName(response[4]));
        Serial.print(" (ID: ");
        Serial.print(response[4], HEX);
        Serial.print(") ");
      }

      // Interpret paddles
      if (response[7] != 0 || response[8] != 0 || response[9] != 0) {
        if (response[7] != 0) {
          Serial.print(response[7]);
          Serial.print(" (ID: ");
          Serial.print(response[7], HEX);
          Serial.print(") ");
        }
        if (response[8] != 0) {
          Serial.print(getShifteId(response[8]));
          Serial.print(" (ID: ");
          Serial.print(response[8], HEX);
          Serial.print(") ");
        }
        if (response[9] != 0) {
          Serial.print("horn: ");
          Serial.print(response[9], HEX);
        }
      }

      Serial.println();
    }
  }
}

const char* getButtonName(byte id) {
  // Return button name from keyMapping
  if (id < sizeof(keyMapping) / sizeof(keyMapping[0])) {
    return keyMapping[id];
  }
  return "Unknown";
}

const char* getShifteId(byte id) {
  // Return shifter id  from shifterMapping
  if (id < sizeof(shifterMapping) / sizeof(shifterMapping[0])) {
    return shifterMapping[id];
  }
  return "Unknown";
}

byte calculateParity(byte id) {
  // Calculate the parity bits for the PID
  byte p0 = ((id >> 0) ^ (id >> 1) ^ (id >> 2) ^ (id >> 4)) & 0x01; // XOR bits 0, 1, 2, 4
  byte p1 = (~((id >> 1) ^ (id >> 3) ^ (id >> 4) ^ (id >> 5))) & 0x01; // XOR bits 1, 3, 4, 5 and invert
  return (id & 0x3F) | (p0 << 6) | (p1 << 7);  // Combine ID and parity bits
}

byte calculateEnhancedChecksum(byte pid, byte *data, int length) {
  // Enhanced checksum: sum of PID and data bytes
  unsigned int sum = pid;  // Start with PID in the checksum

  // Add data bytes to checksum
  for (int i = 0; i < length; i++) {
    sum += data[i];
  }

  // Fold sum into 8 bits
  while (sum > 0xFF) {
    sum = (sum & 0xFF) + (sum >> 8);
  }

  // Invert the result
  return ~((byte)sum);
}