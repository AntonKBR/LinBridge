#include "lin_lib.h"
#include "can_lib.h"

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
    while (!Serial);
    uartSetup();  // Setup UART for LIN communication
    LOG("LIN Bridge Initialized");
    canInit();  // Initialize CAN interface
}

void loop() {
    byte response[BUFFER_SIZE];
    int index = 0;

    sendIgnitionFrame();
    sendButtonRequestFrame();
    listenForResponse(response, index);  // Listen for responses and parse them
    parseResponse(response, index);  // You may need to adjust parsing to integrate CAN sending
    sendAccRequestFrame();
    listenForResponse(response, index);
    parseResponse(response, index);  // As above, include CAN send function calls as needed
}