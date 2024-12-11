#include "lin_lib.h"

// Mappings
const char* getShifterName(byte id) {
    switch (id) {
        case 0x01: return "Shift -";
        case 0x02: return "Shift +";
        case 0x03: return "Shift +/-";
        default: return "Unknown";
    }
}

const char* getButtonName(byte id) {
    switch (id) {
        case 0x02: return "Src+";
        case 0x03: return "Src-";
        case 0x04: return "Arrow Up";
        case 0x05: return "Arrow Down";
        case 0x07: return "OK";
        case 0x10: return "Volume+";
        case 0x11: return "Volume-";
        case 0x15: return "Next";
        case 0x16: return "Previous";
        case 0x19: return "Voice";
        case 0x23: return "View";
        default: return "Unknown";
    }
}

const char* getAccButtonName(byte id) {
    switch (id) {
        case 0x81: return "ACC Set";
        case 0x82: return "ACC Speed +";
        case 0x84: return "ACC Speed -";
        case 0xB0: return "ACC Distance";
        case 0x88: return "ACC Resume";
        case 0x80: return "ACC Mode (Ignored)";
        default: return "Unknown ACC Button";
    }
}

const char* getAccStateName(byte id) {
    switch (id) {
        case 0x2A: return "ACC On/Off";
        case 0x2B: return "ACC Idle";
        default: return "Unknown ACC State";
    }
}

// LIN Logic Functions
void sendBreakSignal() {
    Serial1.end();
    Serial1.begin(9600, SERIAL_8N1);  // Temporarily lower baud rate
    Serial1.write(0x00);              // Send a null byte (dominant signal)
    Serial1.end();
    Serial1.begin(19200, SERIAL_8N1); // Restore LIN baud rate
}

byte calculateParity(byte id) {
    byte p0 = ((id >> 0) ^ (id >> 1) ^ (id >> 2) ^ (id >> 4)) & 0x01;
    byte p1 = (~((id >> 1) ^ (id >> 3) ^ (id >> 4) ^ (id >> 5))) & 0x01;
    return (id & 0x3F) | (p0 << 6) | (p1 << 7);
}

byte calculateEnhancedChecksum(byte pid, byte* data, int length) {
    unsigned int sum = pid;
    for (int i = 0; i < length; i++) {
        sum += data[i];
    }
    while (sum > 0xFF) {
        sum = (sum & 0xFF) + (sum >> 8);
    }
    return ~((byte)sum);
}

void sendIgnitionFrame() {
    byte rawId = 0x0D;
    byte pid = calculateParity(rawId);
    byte data[] = {0x64, 0x81, 0xFF, 0xFF};
    byte checksum = calculateEnhancedChecksum(pid, data, sizeof(data));

    sendBreakSignal();
    Serial1.write(0x55);  // Sync Byte
    Serial1.write(pid);   // PID

    for (int i = 0; i < sizeof(data); i++) {
        Serial1.write(data[i]);
    }

    Serial1.write(checksum);
}

void sendButtonRequestFrame() {
    byte rawId = 0x8E;
    byte pid = calculateParity(rawId);

    sendBreakSignal();
    Serial1.write(0x55);  // Sync Byte
    Serial1.write(pid);   // PID
}

void sendAccRequestFrame() {
    byte rawId = 0x0F;
    byte pid = calculateParity(rawId);

    sendBreakSignal();
    Serial1.write(0x55);  // Sync Byte
    Serial1.write(pid);   // PID
}

void listenForResponse(byte* response, int& index) {
    unsigned long startTime = millis();
    index = 0;

    while (millis() - startTime < LIN_TIMEOUT) {
        if (Serial1.available()) {
            response[index++] = Serial1.read();
            if (index >= BUFFER_SIZE) break;
        }
    }
}

void parseResponse(byte* response, int length) {
    if (length < 2) {
        Serial.println("Response too short to parse.");
        return;
    }

    if (response[1] == 0x8E) {
        if (response[3] != 0) {
            Serial.print("Button 1: ");
            Serial.println(getButtonName(response[3]));
        }
        if (response[4] != 0) {
            Serial.print("Button 2: ");
            Serial.println(getButtonName(response[4]));
        }
    } else if (response[1] == 0xCF) {
        if (response[4] != 0x80) {
            Serial.print("ACC Button: ");
            Serial.println(getAccButtonName(response[4]));
        }
        if (response[5] != 0x2B) {
            Serial.print("ACC State: ");
            Serial.println(getAccStateName(response[5]));
        }
    }
}