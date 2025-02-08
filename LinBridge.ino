#include "./src/lin/lin_lib.h"
#include "./src/can/can_lib.h"
#include "./src/btn_press_handler/btn_handler.h"

// Constants for LIN communication
#define NSLP_PIN 4

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

    // Set NSLP pin to HIGH to enable the TJA1020 transceiver
    pinMode(NSLP_PIN, OUTPUT);  // Configure NSLP_PIN as output
    digitalWrite(NSLP_PIN, HIGH);  // Set NSLP_PIN to HIGH for normal operation

    setupButtons();  // Setup GPIO pins for buttons

    uartSetup();  // Setup UART for LIN communication
    LOG("LIN Bridge Initialized");
    canInit();  // Initialize CAN interface
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