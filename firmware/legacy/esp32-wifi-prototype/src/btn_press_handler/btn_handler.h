#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <Arduino.h>

// Define GPIO pins for buttons
#define ACC_SPEED_UP_PIN  12
#define ACC_SPEED_DOWN_PIN 11
#define ACC_SET_PIN       10
#define MFA_UP_PIN       9  
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
void handleButtonState(uint8_t btnID, bool press);
String getButtonLogs();

#endif // BUTTON_HANDLER_H