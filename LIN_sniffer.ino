#define LIN_TX_PIN 18 // TX1 on Arduino Mega
#define LIN_RX_PIN 19 // RX1 on Arduino Mega
#define SLP_PIN 8     // Sleep control pin for the LIN transceiver (optional)

// Initialize Serial1 for LIN communication
void setup() {
  pinMode(SLP_PIN, OUTPUT);
  digitalWrite(SLP_PIN, HIGH); // Wake up the LIN transceiver
  
  Serial.begin(115200);       // For debugging via Serial Monitor
  Serial1.begin(10417);       // LIN communication at 10417 baud
  Serial.println("LIN Master Initialized");
}

void loop() {
  sendLINFrame();  // Send a LIN frame to the slave
  delay(10);       // Short delay for the slave to respond
  listenForResponse(); // Listen for the slave response
  delay(750);      // Slightly reduced delay before the next frame is sent
}

void sendLINFrame() {
  // Frame components
  byte id = 0x8E; // LIN ID for the target device
  byte data[] = {0x14, 0x00, 0x00, 0xA3, 0x00, 0x00, 0x00}; // Data bytes
  byte checksum = calculateChecksum(id, data, sizeof(data));

  // Generate Break Field
  digitalWrite(LIN_TX_PIN, LOW); // Drive TX pin low
  delayMicroseconds(1500);       // Hold low for ~1.5 ms
  digitalWrite(LIN_TX_PIN, HIGH); // Return to idle state

  // Send Sync Byte
  Serial1.write(0x55);

  // Send ID Byte
  Serial1.write(id);

  // Send Data Bytes
  for (byte i = 0; i < sizeof(data); i++) {
    Serial1.write(data[i]);
  }

  // Send Checksum Byte
  Serial1.write(checksum);

  // Debugging output
  Serial.print("Sent Frame: [ID: ");
  Serial.print(id, HEX);
  Serial.print("] Data: ");
  for (byte i = 0; i < sizeof(data); i++) {
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.print("Checksum: ");
  Serial.println(checksum, HEX);
}

// Function to calculate the LIN checksum
byte calculateChecksum(byte id, byte *data, int length) {
  byte checksum = id; // Start with the ID for classic checksum
  for (int i = 0; i < length; i++) {
    checksum += data[i];
  }
  checksum = checksum % 256; // Modulo 256
  return ~checksum;          // Return 1's complement
}

// Function to listen for a LIN slave response
void listenForResponse() {
  if (Serial1.available()) {
    Serial.print("Response: ");
    while (Serial1.available()) {
      byte received = Serial1.read();
      Serial.print("0x");
      Serial.print(received, HEX);
      Serial.print(" ");
    }
    Serial.println();
  } else {
    Serial.println("No Response Received");
  }
}