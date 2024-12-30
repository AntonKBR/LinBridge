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

void translateToCan(uint8_t *data, uint8_t len) {
    if (len > 8) len = 8; // CAN frames have a max data length of 8 bytes

    twai_message_t message;
    message.identifier = CAN_ID;
    message.extd = 0; // Standard frame
    message.data_length_code = len;
    for (int i = 0; i < len; i++) {
        message.data[i] = data[i];
    }

    if (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
        Serial.println("Data transmitted to CAN bus");
    } else {
        Serial.println("Failed to transmit data to CAN bus");
    }
}