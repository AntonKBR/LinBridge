#include <SoftwareSerial.h>

#define LIN_TX_PIN 10
#define LIN_RX_PIN 9
#define SLP_PIN 8

SoftwareSerial linSerial(LIN_RX_PIN, LIN_TX_PIN);

byte counter = 0x10; // Initialize the counter (0x10 as in example)

// Sends a wake-up signal
void sendWakeUpSignal() {
    for (int i = 0; i < 11; i++) {
        linSerial.write((uint8_t)0x00); // Send break signal
    }
    linSerial.write(0x80); // Send wake-up command
    Serial.println("Master Debug: Wake-Up Signal Sent (0x80)");
}

// Calculate Classic checksum (data bytes only)
byte calculateChecksum(byte *data, int length) {
    byte checksum = 0;
    for (int i = 0; i < length; i++) {
        checksum += data[i];
    }
    return ~checksum; // One's complement
}

// Send a LIN frame
void sendLINFrame(byte id, byte *data, int length) {
    linSerial.write(0x55); // Sync byte
    linSerial.write(id);   // Frame ID
    for (int i = 0; i < length; i++) {
        linSerial.write(data[i]); // Data bytes
    }
    byte checksum = calculateChecksum(data, length); // Calculate checksum
    linSerial.write(checksum); // Checksum byte

    // Debug output
    Serial.print("Master Debug: Frame Sent: 0x55 0x");
    Serial.print(id, HEX);
    for (int i = 0; i < length; i++) {
        Serial.print(" 0x");
        Serial.print(data[i], HEX);
    }
    Serial.print(" 0x");
    Serial.println(checksum, HEX);
}

// Listen for LIN response
void listenForResponse() {
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
        Serial.println("Master Debug: No Response Received");
    }
}

void setup() {
    pinMode(SLP_PIN, OUTPUT);
    digitalWrite(SLP_PIN, HIGH); // Activate LIN transceiver
    Serial.begin(115200);
    linSerial.begin(10417); // LIN baud rate
    Serial.println("Master Debug: LIN Initialized");
}

void loop() {
    sendWakeUpSignal();
    delay(50); // Delay for device wake-up

    // Poll Steering Wheel Buttons
    byte buttonData[] = {counter, 0x00, 0x00, 0xA3, 0x00, 0x00, 0x00};
    sendLINFrame(0x8E, buttonData, 7);
    delay(32); // Ensure minimum delay of 32ms
    listenForResponse();

    // Increment the counter
    counter++;
    if (counter > 0x1F) { // Reset counter after 0x1F
        counter = 0x10;
    }

    // Delay before the next polling cycle
    delay(200); // Longer delay to avoid overwhelming the LIN bus
}