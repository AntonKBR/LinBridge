#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <Arduino.h>

// Define GPIO pins for buttons
#define MFA_UP_PIN       9  // Example GPIO
#define MFA_DOWN_PIN     8
#define MFA_RESET_PIN    7

// Structure for mapping button IDs to GPIO pins
struct ButtonMapping {
    byte btnID;  // Button Byte ID
    int pin;     // ESP32 GPIO Pin
};

// Declare button mappings as `extern` to avoid multiple definitions
extern ButtonMapping buttonMappings[];

// Function Declarations (only declarations here!)
void setupButtons();
int getButtonPin(uint8_t btnID);
void handleFirstButtonPress(uint8_t btnID);
void handleFirstButtonRelease(uint8_t btnID);
void handleSecondButtonPress(uint8_t btnID);
void handleSecondButtonRelease(uint8_t btnID);

#endif // BUTTON_HANDLER_H