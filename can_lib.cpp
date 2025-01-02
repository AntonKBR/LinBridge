// can_lib.cpp
#include "can_lib.h"

void canInit() {
    // Initialize TWAI (CAN) with default settings
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(CAN_TX, CAN_RX, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    if (twai_driver_install(&g_config, &t_config, &f_config) != ESP_OK) {
        Serial.println("Failed to install TWAI driver");
        return;
    }

    if (twai_start() != ESP_OK) {
        Serial.println("Failed to start TWAI driver");
        return;
    }

    Serial.println("CAN driver initialized successfully");
}

void translateToCan(uint8_t *data) {
    static unsigned long lastTransmission = 0;
    unsigned long currentMillis = millis();
    
    // Ensure there is a minimum interval between transmissions
    if (currentMillis - lastTransmission < 100) return;  // 100ms interval

    twai_message_t message;
    message.identifier = CAN_ID;
    message.extd = 0; // Standard frame
    message.data_length_code = 4;
    message.data[0] = data[3];
    message.data[1] = data[4];
    message.data[2] = data[5];
    message.data[3] = 0x11;

    Serial.println("Starting transmission");
    if (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_FAIL) {
        Serial.println("Failed to transmit first frame");
        return; // Exit if transmission fails
    }
    Serial.println("First frame transmitted");
    // Prepare the repetitive message
    message.data[0] = 0x00;
    message.data[1] = 0x00;
    message.data[2] = 0x00;
    message.data[3] = 0x22;

    for (size_t i = 0; i < 3; i++) {
      if (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_FAIL) {
          Serial.println("Failed to transmit repetitive frame");
          return; // Exit if any transmission fails
      }
    }
    Serial.println("Repetitive frame transmitted");
}