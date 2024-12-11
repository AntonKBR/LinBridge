#include <Arduino.h>
#include "lin_lib.h"

#define DEBUG 1

#if DEBUG
#define LOG(x) Serial.println(x)
#define LOGV(x) Serial.print(x)
#else
#define LOG(x)
#define LOGV(x)
#endif

void setup() {
    Serial.begin(115200);              // Debugging output
    Serial1.begin(19200, SERIAL_8N1);  // LIN communication at 19,200 baud
    LOG("LIN Master Initialized");
}

void loop() {
    byte response[BUFFER_SIZE];
    int index = 0;

    sendIgnitionFrame();
    sendButtonRequestFrame();
    listenForResponse(response, index);
    parseResponse(response, index);

    sendAccRequestFrame();
    listenForResponse(response, index);
    parseResponse(response, index);
}