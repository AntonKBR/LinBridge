# Legacy ESP32 Wi-Fi prototype firmware

This directory preserves the firmware previously stored at the repository root.
Its source files were moved without functional modification.

Important limitations:

- this firmware is not canonical for the square LinBridge 2.0 PCB;
- its pinout belongs to an earlier experimental configuration;
- the CAN identifier, bitrate and payload are experimental and unverified;
- the fixed Wi-Fi password is intended only for an isolated bench setup;
- the HTTP diagnostic page is unauthenticated;
- the code is retained for project history and comparison with the firmware
  eventually verified on LinBridge 2.0.

Do not use this firmware as evidence that CAN, automatic brightness, KEY1/KEY2
or the LinBridge 2.0 pinout have been validated.
