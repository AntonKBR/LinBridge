#include "./src/lin/lin_lib.h"
#include "./src/can/can_lib.h"
#include "./src/btn_press_handler/btn_handler.h"

#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char *ssid = "LinBridge";
const char *password = "12345678";

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

// Storage for button press logs
String buttonLog = "";

void logButtonPress(const char *buttonName) {
    String logEntry = String(millis() / 1000) + "s: " + buttonName + "<br>";
    buttonLog = logEntry + buttonLog;
    if (buttonLog.length() > 2000) {  // Prevent memory overflow
        buttonLog = buttonLog.substring(0, 2000);
    }
}

void setup() {
    Serial.begin(115200);
    while (!Serial);

    pinMode(NSLP_PIN, OUTPUT);
    digitalWrite(NSLP_PIN, HIGH);

    setupButtons();
    uartSetup();
    LOG("LIN Bridge Initialized");
    canInit();

    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    Serial.println("WiFi AP Started! Visit http://192.168.4.1");

    // Serve logs
    server.on("/log", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", getButtonLogs());
    });

    // Serve homepage with auto-refresh logs
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html",
                      "<h1>LinBridge Logs</h1>"
                      "<div id='log'></div>"
                      "<script>setInterval(()=>{fetch('/log').then(res=>res.text()).then(txt=>document.getElementById('log').innerHTML=txt)}, 1000);</script>");
    });

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
