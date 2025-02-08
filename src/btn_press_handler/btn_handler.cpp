#include "btn_handler.h"

// Define button mappings ONLY ONCE in this file
ButtonMapping buttonMappings[] = {
    {0x04, MFA_UP_PIN},
    {0x05, MFA_DOWN_PIN},
    {0x07, MFA_RESET_PIN}
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

// Handle Button Press
void handleFirstButtonPress(uint8_t btnID) {
    int pin = getButtonPin(btnID);
    if (pin != -1) {
        digitalWrite(pin, HIGH);
    }
}

//Handle Button Release
void handleFirstButtonRelease(uint8_t btnID) {
    int pin = getButtonPin(btnID);
    if (pin != -1) {
        digitalWrite(pin, LOW);
    }
}

// Handle Second Button Press
void handleSecondButtonPress(uint8_t btnID) {
    int pin = getButtonPin(btnID);
    if (pin != -1) {
        digitalWrite(pin, HIGH);
    }
}

// Handle Second Button Release
void handleSecondButtonRelease(uint8_t btnID) {
    int pin = getButtonPin(btnID);
    if (pin != -1) {
        digitalWrite(pin, LOW);
    }
}