#include "btn_handler.h"

// Define button mappings ONLY ONCE in this file
ButtonMapping buttonMappings[] = {
    {0x04, MFA_UP_PIN},
    {0x05, MFA_DOWN_PIN},
    {0x07, MFA_RESET_PIN},
    {0x82, ACC_SPEED_UP_PIN},
    {0x84, ACC_SPEED_DOWN_PIN},
    {0x81, ACC_SET_PIN}
};

// Setup Button Pins
void setupButtons() {
    for (const auto& mapping : buttonMappings) {
        pinMode(mapping.pin, OUTPUT);
        digitalWrite(mapping.pin, LOW);  // Ensure buttons are not pressed at start
    }
}

// Get Button Pin from ID
int getButtonPin(uint8_t btnID) {
    for (const auto& mapping : buttonMappings) {
        if (mapping.btnID == btnID) {
            return mapping.pin;
        }
    }
    return -1;  // Not found
}

// Generic function to handle button press or release
void handleButtonState(uint8_t btnID, bool press) {
    int pin = getButtonPin(btnID);
    if (pin != -1) {
        digitalWrite(pin, press ? HIGH : LOW);
    }
}