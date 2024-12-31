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
    Serial.begin(115200);
    Serial1.begin(19200, SERIAL_8N1, GPIO_NUM_3, GPIO_NUM_2);
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