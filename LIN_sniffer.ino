#include <SoftwareSerial.h>

#define LIN_TX_PIN 10
#define LIN_RX_PIN 9
#define SLP_PIN 8

SoftwareSerial linSerial(LIN_RX_PIN, LIN_TX_PIN);

void sendWakeUpSignal() {
  for (int i = 0; i < 11; i++) {
    linSerial.write((uint8_t)0x00); // Send break signal
  }
  linSerial.write(0x55); // Sync Byte
  Serial.println("Master Debug: Wake-Up Signal Sent");
  delay(200); // Increased delay for slave to wake up
}

void sendIgnitionFrame() {
  byte frameID = 0x0D; // Frame ID for ignition
  byte data[] = {0x00, 0x80, 0x64, 0x60}; // Ignition data
  byte checksum = frameID;
  for (int i = 0; i < sizeof(data); i++) {
    checksum += data[i];
  }
  checksum = ~checksum;

  linSerial.write(0x55); // Sync Byte
  linSerial.write(frameID);
  for (int i = 0; i < sizeof(data); i++) {
    linSerial.write(data[i]);
  }
  linSerial.write(checksum);

  Serial.print("Master Debug: Ignition Frame Sent: 0x55 0x");
  Serial.print(frameID, HEX);
  for (int i = 0; i < sizeof(data); i++) {
    Serial.print(" 0x");
    Serial.print(data[i], HEX);
  }
  Serial.print(" 0x");
  Serial.println(checksum, HEX);

  delay(200); // Delay for the slave to process the ignition frame
}

void pollButtonBlock() {
  byte frameID = 0x8E; // Button block Frame ID
  byte data[] = {0x10, 0x00, 0x00, 0xA3, 0x00, 0x00, 0x00};
  byte checksum = frameID;
  for (int i = 0; i < sizeof(data); i++) {
    checksum += data[i];
  }
  checksum = ~checksum;

  linSerial.write(0x55); // Sync Byte
  linSerial.write(frameID);
  for (int i = 0; i < sizeof(data); i++) {
    linSerial.write(data[i]);
  }
  linSerial.write(checksum);

  Serial.print("Master Debug: Poll Sent: 0x55 0x");
  Serial.print(frameID, HEX);
  for (int i = 0; i < sizeof(data); i++) {
    Serial.print(" 0x");
    Serial.print(data[i], HEX);
  }
  Serial.print(" 0x");
  Serial.println(checksum, HEX);

  delay(200); // Delay for the slave to respond
}

void receiveResponse() {
  if (linSerial.available()) {
    Serial.print("Master Debug: Response Received: ");
    while (linSerial.available()) {
      byte receivedByte = linSerial.read();
      Serial.print("0x");
      Serial.print(receivedByte, HEX);
      Serial.print(" ");
    }
    Serial.println();
  } else {
    Serial.println("Master Debug: No Response Received");
  }
}

void setup() {
  pinMode(SLP_PIN, OUTPUT);
  digitalWrite(SLP_PIN, HIGH); // Wake up LIN transceiver
  Serial.begin(115200);
  linSerial.begin(10417);
  Serial.println("Master Debug: LIN Initialized");
}

void loop() {
  sendWakeUpSignal();
  sendIgnitionFrame();
  pollButtonBlock();
  receiveResponse();
  delay(1000); // Wait before next cycle
}