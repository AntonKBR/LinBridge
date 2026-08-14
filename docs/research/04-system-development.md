# Electronics, firmware and PCB evolution

## System architecture

The developed concept has four paths:

```text
MQB buttons -> LIN transceiver -> ESP32 -> GPIO emulation -> legacy vehicle inputs
                                  |   |
                                  |   +-> experimental CAN translation
                                  +-----> Wi-Fi bench diagnostics

vehicle illumination voltage -> ADC -> LIN backlight frame -> button illumination
```

## Controller evolution

The conversations considered Arduino Nano/Pro Mini, ESP8266 D1 Mini and STM32 as
development or analysis tools. The repository then converged on ESP32 hardware
UART because it supports deterministic LIN serial handling, CAN experimentation,
ADC input and Wi-Fi diagnostics in one controller.

This distinction matters: not every considered board became part of LinBridge.
STM32/ST-Link and Arduino-as-analyzer discussions describe tooling experiments;
the maintained product firmware is ESP32-oriented.

## LIN physical layer

External LIN-to-TTL adapters were used before custom PCB integration. Candidate
transceivers discussed include TJA1021, SIT1021T and MCP2004-family parts. Exact
component selection must be taken from the BOM/schematic revision being built,
not from a generic chat recommendation.

## CAN path

The CAN module initializes an ESP32 CAN library and translates selected LIN data
into a four-byte standard frame. The code proves architectural experimentation,
but the research archive lacks a retained target-vehicle CAN trace, a verified
message contract and an end-to-end acceptance result. CAN translation is thus
**implemented experimentally**, not validated for vehicle deployment.

## GPIO and resistive output paths

GPIO emulation is the most explicit legacy-integration path in the current code.
Mapped LIN button codes drive dedicated outputs on press and release. The square
LinBridge 2.0 board also includes a digital-potentiometer path for resistive
KEY1/KEY2-style control. Resistance changes were observed with a multimeter, but
the path has not yet been validated with the target head unit and its actual I2C
addresses and required resistance values remain to be established.

## Hardware generations

### Narrow development board

The narrow, two-layer board was the protocol and electronics proof of concept.
It was used to validate the power path, LIN master behavior, wheel wake-up,
button identification, web diagnostics and illumination control. It is retained
as evidence of the engineering process but is frozen and will not receive new
features.

### Square LinBridge 2.0 relay-replacement board

LinBridge 2.0 is a square, two-layer PCB with single-sided component assembly,
designed to replace the internal PCB of the original VAG
multifunction-steering-wheel relay. Its design reuses the relay enclosure,
transferred contact pins and factory mounting location. This is the only active
hardware target for future firmware and validation.

The board is physically assembled and works on the bench. Confirmed functions
include power, ESP32 operation, LIN wake-up and polling, correct identification
of tested buttons in the web interface and basic illumination control. CAN is
not software-complete. Automatic brightness requires refinement. KEY1/KEY2 has
only been checked electrically with a multimeter.

## PCB evolution and production state

The hardware archive demonstrates repeated schematic/layout/manufacturing
iteration, including capacitor/package discussions and EasyEDA workflow choices.
For production traceability:

- latest editable source, schematic, PCB view and BOM: 2025-09-01;
- latest discovered Gerber package: 2025-09-09;
- the 2025-09-09 Gerber differs from the editable-source export and lacks a top
  silkscreen file (`.GTO`);
- no later editable source was found in the local archive; the known supplier
  order still needs to be recovered and added.

The missing top silkscreen was an intentional aesthetic decision. A supplier
order for the latest physical batch exists and should be used to recover the
exact production Gerber ZIP. Until that comparison is complete, the production
archive must be reviewed as a set rather than chosen solely by filename.

A newer EasyEDA 6.5.57 Gerber export dated 2026-08-14 16:11:16 has been
inspected as a canonical-package candidate. It identifies the design as
`LinBridge_2.0 / PCB_LinBridge_2.0`, has an approximate outline of
60.69 x 53.06 mm and includes outline, drill, paste, mask, silkscreen and both
top and bottom copper layers. Its SHA-256 is
`8598ad556c318cc92d715cbddcf104788f174041a3f67bb990e6780b995cbbb6`.
The large plated-hole rows are consistent with the transferred relay contacts.
This verifies design identity and two-layer construction, but not yet identity
with the Gerber used for the latest assembled production batch.

## Diagnostic interface caveat

The current Wi-Fi branch uses a fixed SSID/password and an unauthenticated HTTP
page. It is suitable only for an isolated bench. Credentials, network exposure,
memory ownership and long-running stability require redesign before portfolio
demonstration or vehicle use.
