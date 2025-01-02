#include "can_lib.h"

void canInit() {
    Serial.begin(115200);
    ESP32Can.setPins(CAN_TX, CAN_RX);  // Assuming setPins is a valid method
    ESP32Can.setSpeed(ESP32Can.convertSpeed(500));

    if (ESP32Can.begin()) {  
        Serial.println("ESP32Can bus started!");
    } else {
        Serial.println("Failed to start ESP32Can bus!");
    }
}

void translateToCan(uint8_t *data) {
    CanFrame frame;
    frame.identifier = CAN_ID;  // Ensure CAN_ID is defined
    frame.extd = 0;
    frame.data_length_code = 4;
    frame.data[0] = data[3];
    frame.data[1] = data[4];
    frame.data[2] = data[5];
    frame.data[3] = 0x11;  

    if (ESP32Can.writeFrame(frame)) {
        Serial.println("Message sent successfully");
    } else {
        Serial.println("Failed to send message");
    }
}