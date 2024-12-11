#define LIN_TX_PIN 1  // TX1 pin on Arduino Mega (Hardware Serial1)

#define LIN_TIMEOUT 75  // Timeout for LIN response in milliseconds
#define BUFFER_SIZE 16  // Buffer size for LIN response
#define DEBUG 1         // Set to 0 to disable debug messages

#if DEBUG
#define LOG(x) Serial.println(x)
#define LOGV(x) Serial.print(x)
#else
#define LOG(x)
#define LOGV(x)
#endif

// Shifter Mapping
const char* getShifterName(byte id) {
  switch (id) {
    case 0x01: return "Shift -";
    case 0x02: return "Shift +";
    case 0x03: return "Shift +/-";
    default: return "Unknown";
  }
}

// Button Name Mapping with switch-case
const char* getButtonName(byte id) {
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

// ACC Button Mapping for Byte [4]
const char* getAccButtonName(byte id) {
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

// ACC Button Mapping for Byte [5]
const char* getAccStateName(byte id) {
  switch (id) {
    case 0x2A: return "ACC On/Off";
    case 0x2B: return "ACC Idle";
    default: return "Unknown ACC State";
  }
}


void setup() {
  Serial.begin(115200);              // Debugging output
  Serial1.begin(19200, SERIAL_8N1);  // LIN communication at 19,200 baud
  LOG("LIN Master Initialized");
}

void loop() {
  sendIgnitionFrame();       // Send the Ignition Frame (0x0D)
  sendButtonRequestFrame();  // Send the Button Request Frame (0x8E)
  listenForResponse();       // Listen for LIN response
  sendAccRequestFrame();
  listenForResponse();
}

// Function to send a LIN Break Signal
void sendBreakSignal() {
  Serial1.end();
  Serial1.begin(9600, SERIAL_8N1);  // Temporarily lower baud rate
  Serial1.write(0x00);              // Send a null byte (dominant signal)
  Serial1.end();
  Serial1.begin(19200, SERIAL_8N1);  // Restore LIN baud rate
}

// Function to send the Ignition Frame
void sendIgnitionFrame() {
  byte rawId = 0x0D;
  byte pid = calculateParity(rawId);
  byte data[] = { 0x64, 0x81, 0xFF, 0xFF };  // Example data: Illumination and accessory status
  byte checksum = calculateEnhancedChecksum(pid, data, sizeof(data));

  sendBreakSignal();
  Serial1.write(0x55);  // Sync Byte
  Serial1.write(pid);   // PID

  for (int i = 0; i < sizeof(data); i++) {
    Serial1.write(data[i]);
  }

  Serial1.write(checksum);
}

// Function to send a Button Request Frame
void sendButtonRequestFrame() {
  byte rawId = 0x8E;
  byte pid = calculateParity(rawId);

  sendBreakSignal();
  Serial1.write(0x55);  // Sync Byte
  Serial1.write(pid);   // PID
}

// Function to send ACC Button Request Frame
void sendAccRequestFrame() {
  byte rawId = 0x0F;
  byte pid = calculateParity(rawId);

  sendBreakSignal();
  Serial1.write(0x55);  // Sync Byte
  Serial1.write(pid);   // PID
}

// Function to listen and parse LIN responses
void listenForResponse() {
  byte response[BUFFER_SIZE] = { 0 };
  int index = 0;
  unsigned long startTime = millis();

  while (millis() - startTime < LIN_TIMEOUT) {
    if (Serial1.available()) {
      response[index++] = Serial1.read();
      if (index >= BUFFER_SIZE) break;  // Prevent buffer overflow
    }
  }

  if (index > 0) {
    parseResponse(response, index);
  }
}

// Function to parse and display LIN response
void parseResponse(byte* response, int length) {
  if (response[1] == 0x8E && (response[3] != 0 || response[4] != 0 || response[5] != 0 || response[7] != 0 || response[8] != 0 || response[9] != 0)) {

    // Display button presses
    if (response[3] != 0) {
      LOGV(getButtonName(response[3]));
      LOG(" (ID: " + String(response[3], HEX) + ")");
    }
    if (response[4] != 0) {
      LOGV(getButtonName(response[4]));
      LOG(" (ID: " + String(response[4], HEX) + ")");
    }

    // Display paddles and horn
    if (response[7] != 0) {
      LOG("Left Paddle: " + String(getShifterName(response[7])));
    }
    if (response[8] != 0) {
      LOG("Right Paddle: " + String(getShifterName(response[8])));
    }
    if (response[9] != 0) {
      LOG("Horn Pressed");
    }
  }

  if (response[1] == 0xCF) {
    if (response[4] != 0x80) {
      LOGV(getAccButtonName(response[4]));
      LOG(" (ID: " + String(response[4], HEX) + ")");
    }
    if (response[5] != 0x2B) {
      LOGV(getAccStateName(response[5]));
      LOG(" (ID: " + String(response[5], HEX) + ")");
    }
  }
}

// Function to calculate parity bits
byte calculateParity(byte id) {
  byte p0 = ((id >> 0) ^ (id >> 1) ^ (id >> 2) ^ (id >> 4)) & 0x01;
  byte p1 = (~((id >> 1) ^ (id >> 3) ^ (id >> 4) ^ (id >> 5))) & 0x01;
  return (id & 0x3F) | (p0 << 6) | (p1 << 7);
}

// Function to calculate the enhanced checksum
byte calculateEnhancedChecksum(byte pid, byte* data, int length) {
  unsigned int sum = pid;
  for (int i = 0; i < length; i++) {
    sum += data[i];
  }
  while (sum > 0xFF) {
    sum = (sum & 0xFF) + (sum >> 8);
  }
  return ~((byte)sum);
}