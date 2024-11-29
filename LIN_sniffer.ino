#include <SoftwareSerial.h>

#define LIN_TX_PIN 10
#define LIN_RX_PIN 9
#define SLP_PIN 8

SoftwareSerial linSerial(LIN_RX_PIN, LIN_TX_PIN);

byte calculateChecksum(byte frameID, byte data[], int length) {
  byte checksum = frameID;
  for (int i = 0; i < length; i++) {
    checksum += data[i];
  }
  return ~checksum;
}

void sendWakeUpSignal() {
  for (int i = 0; i < 11; i++) {
    linSerial.write((uint8_t)0x00); // Send break signal
  }
  Serial.println("Master Debug: Wake-Up Signal Sent");
}

void sendLINFrame(byte frameID, byte data[], int length) {
  byte checksum = calculateChecksum(frameID, data, length);
  linSerial.write(0x55); // Sync Byte
  linSerial.write(frameID); // Frame ID
  for (int i = 0; i < length; i++) {
    linSerial.write(data[i]); // Data bytes
  }
  linSerial.write(checksum); // Checksum

  // Debug output
  Serial.print("Master Debug: Frame Sent: 0x55 0x");
  Serial.print(frameID, HEX);
  for (int i = 0; i < length; i++) {
    Serial.print(" 0x");
    Serial.print(data[i], HEX);
  }
  Serial.print(" 0x");
  Serial.println(checksum, HEX);
}

void readResponse() {
  if (linSerial.available()) {
    Serial.print("Master Debug: Response Received: ");
    while (linSerial.available()) {
      byte received = linSerial.read();
      Serial.print("0x");
      Serial.print(received, HEX);
      Serial.print(" ");
    }
    Serial.println();
  } else {
    Serial.println("Master Debug: No Response");
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
  byte ignitionData[] = {0x1A, 0x00, 0x00, 0x64, 0x60};
  byte pollData[] = {0x10, 0x00, 0x00, 0xA3, 0x00, 0x00, 0x00};

  sendWakeUpSignal();
  delay(50); // Ensure sufficient wake-up delay

  sendLINFrame(0x0D, ignitionData, 5); // Ignition frame
  delay(20); // Short delay
  readResponse();

  sendLINFrame(0x8E, pollData, 7); // Poll frame
  delay(20); // Short delay
  readResponse();

  delay(200); // Delay before next loop
}