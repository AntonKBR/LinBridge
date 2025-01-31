#ifndef LIN_LIB_H
#define LIN_LIB_H

#include <Arduino.h>
#include <HardwareSerial.h>

extern HardwareSerial SerialLIN;  // Use UART1 for LIN (declaration only)

// Constants for LIN communication
#define LIN_TX_PIN 2  
#define LIN_RX_PIN 3

#define LIN_TIMEOUT 50  // Timeout for LIN response in milliseconds
#define BUFFER_SIZE 16   // Buffer size for LIN response

// Function Declarations
void uartSetup(); 
void sendBreakSignal();
byte calculateParity(byte id);
byte calculateEnhancedChecksum(byte pid, byte* data, int length);
void sendIgnitionFrame();
void sendButtonRequestFrame();
void sendAccRequestFrame();
void listenForResponse(byte* response, int& index);
void parseResponse(byte* response, int length);

// Mappings
const char* getShifterName(byte id);
const char* getButtonName(byte id);
const char* getAccButtonName(byte id);
const char* getAccStateName(byte id);

#endif // LIN_LIB_H