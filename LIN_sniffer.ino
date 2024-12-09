#define LIN_TX_PIN 1  // TX1 pin on Arduino Mega (Hardware Serial1)

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
      if (response[3] != 0 || response[4] != 0 || response[5] != 0) {
        Serial.print("Key 1: ");
        Serial.print(response[3], HEX);
        Serial.print(", Key 2: ");
        Serial.print(response[4], HEX);
        Serial.print(", Press Type: ");
        Serial.println(response[5], HEX);
      }

      if (response[7] != 0 || response[8] != 0 || response[9] != 0) {
        Serial.print("Paddle(s) pressed: ");
        Serial.print(response[7], HEX);
        Serial.print(", ");
        Serial.print(response[8], HEX);
        Serial.print(", ");
        Serial.println(response[9], HEX);
      }
      Serial.println();
    }
  }
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