#include "btn_handler.h"
#include <vector>
#include <Arduino.h>

// Define button mappings
ButtonMapping buttonMappings[] = {
    {0x04, MFA_UP_PIN},
    {0x05, MFA_DOWN_PIN},
    {0x07, MFA_RESET_PIN},
    {0x82, ACC_SPEED_UP_PIN},
    {0x84, ACC_SPEED_DOWN_PIN},
    {0x81, ACC_SET_PIN}
};

// Store logs in a buffer
std::vector<String> buttonLogs;

void setupButtons() {
    for (const auto& mapping : buttonMappings) {
        pinMode(mapping.pin, OUTPUT);
        digitalWrite(mapping.pin, LOW);
    }
}

// Get Button Pin from ID
int getButtonPin(uint8_t btnID) {
    for (const auto& mapping : buttonMappings) {
        if (mapping.btnID == btnID) {
            return mapping.pin;
        }
    }
    return -1; 
}

// Handle button press/release and store event
void handleButtonState(uint8_t btnID, bool press) {
    int pin = getButtonPin(btnID);
    if (pin != -1) {
        digitalWrite(pin, press ? HIGH : LOW);
        
        // Store log entry (max 10 logs)
        if (buttonLogs.size() > 10) buttonLogs.erase(buttonLogs.begin());
        buttonLogs.push_back(String(millis()) + ": " + (press ? "Pressed " : "Released ") + String(btnID, HEX));
    }
}

// Function to get logs
String getButtonLogs() {
    String logData;
    for (const auto& log : buttonLogs) {
        logData += log + "<br>";
    }
    return logData;
}