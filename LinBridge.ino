#include "./src/lin/lin_lib.h"
#include "./src/can/can_lib.h"
#include "./src/btn_press_handler/btn_handler.h"

#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// WiFi Credentials
const char *ssid = "LinBridge";
const char *password = "12345678";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

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
    pinMode(NSLP_PIN, OUTPUT);  
    digitalWrite(NSLP_PIN, HIGH);  

    setupButtons();  // Setup GPIO pins for buttons

    uartSetup();  
    LOG("LIN Bridge Initialized");
    canInit();  

    // Start WiFi in Access Point mode
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);

    Serial.println("WiFi AP Started!");
    Serial.println(WiFi.softAPIP());  // Print AP IP Address

    // Serve a simple web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", "<h1>Hello LinBridge!!!</h1>");
    });

    // Start server
    server.begin();
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