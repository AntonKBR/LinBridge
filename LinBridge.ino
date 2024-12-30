#include <Arduino.h>
#include "lin_lib.h"
#include "can_lib.h"

#define DEBUG 1
#define LIN_TX 2
#define LIN_RX 3

#if DEBUG
#define LOG(x) Serial.println(x)
#define LOGV(x) Serial.print(x)
#else
#define LOG(x)
#define LOGV(x)
#endif

void setup() {
    Serial.begin(115200);
    Serial1.begin(19200, SERIAL_8N1, LIN_RX, LIN_TX);
    LOG("LIN Master Initialized");

    canInit(); 
}

void loop() {
    byte response[BUFFER_SIZE];
    int index = 0;

    sendIgnitionFrame();
    sendButtonRequestFrame();
    listenForResponse(response, index);
    parseResponse(response, index);
    translateToCan(response, index); 

    sendAccRequestFrame();
    listenForResponse(response, index);
    parseResponse(response, index);
    translateToCan(response, index); 
}