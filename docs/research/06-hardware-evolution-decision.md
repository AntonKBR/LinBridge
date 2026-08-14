# Hardware evolution and canonical-platform decision

## Decision

Future development targets only the square, two-layer LinBridge 2.0 PCB with
single-sided component assembly. It is a
relay-replacement PCB. The narrow, two-layer board is a completed development
prototype and remains in the record as evidence of the project's evolution.

This decision separates two different engineering phases rather than treating
all PCB artifacts as revisions of one product.

## Phase A - narrow development board

Purpose:

- validate the power and LIN physical layers;
- reproduce LIN master timing and wake-up;
- poll and decode steering-wheel responses;
- identify tested buttons in the ESP32 web interface;
- exercise button illumination;
- provide accessible points for bench debugging.

Status:

- assembled and functional;
- completed its proof-of-concept role;
- frozen as a historical artifact;
- excluded from new feature development and canonical releases.

## Phase B - square LinBridge 2.0

LinBridge 2.0 is designed to replace the original PCB inside the factory VAG
multifunction-steering-wheel relay:

1. Remove the original relay PCB.
2. Transfer the original contact pins to LinBridge 2.0.
3. Install the new board in the original enclosure.
4. Make the required vehicle-wiring changes.
5. Return the module to its factory mounting position.

The approach preserves the original enclosure, connector geometry, installation
location and an OEM-style appearance. It avoids a separate visible control box.

## Evidence-based feature status

| Function | Status | Evidence boundary |
| --- | --- | --- |
| Power supply | Confirmed working | Physical assembled board operated on bench |
| ESP32 and web UI | Confirmed working | Correct tested-button identification is displayed |
| LIN wake-up and polling | Confirmed working | Wheel wakes and button values are received |
| LIN button decoding | Confirmed for tested controls | A canonical annotated trace is still desirable |
| Button illumination | Basic control confirmed | Automatic input-to-brightness calibration remains open |
| MFA/Info outputs | Up, Down and Reset are the intended functions | Final `0x04/0x05/0x07` evidence should be retained |
| KEY1/KEY2 | Electrical response observed with multimeter | Target head unit and I2C addressing not validated |
| CAN | Hardware path present | Bitrate, ID, payload and end-to-end software unverified |

## Canonical firmware rule

Canonical firmware is the version demonstrated to match the assembled square
LinBridge 2.0 board. It is not selected solely by folder date. The newer
PlatformIO project is the preferred structural candidate, but its pin mappings
and experimental CAN, CCS, illumination and resistive paths must be reconciled
with the actual board and the firmware used in the successful LIN test.

No modern firmware target is required for the narrow development board.

## Traceability actions

Before a hardware release:

- recover the Gerber ZIP from the latest supplier order;
- match it to the assembled square PCB and calculate SHA-256;
- identify the installed MCP4018 variant and real I2C address or addresses;
- record annotated photographs of both PCB sides;
- bind schematic, PCB source, BOM, Gerber and firmware to one release manifest;
- record the exact firmware source or commit used for the working LIN test.

The inspected 2026-08-14 Gerber export is the current canonical manufacturing
candidate, not yet a confirmed production revision. It contains both `.GTL` and
`.GBL` copper layers, plated holes and the expected relay-contact geometry. Its
SHA-256 is
`8598ad556c318cc92d715cbddcf104788f174041a3f67bb990e6780b995cbbb6`.
Confirmation requires a match against the supplier-order Gerber or production
preview and the assembled square board.

The historical 2025-09-09 Gerber candidate intentionally omits top silkscreen.
The newer 2026-08-14 candidate includes a top-silkscreen file. Neither candidate
is a confirmed production package until matched to the last supplier order.

## Active development scope

Work on LinBridge 2.0 should proceed in this order:

1. identify the tested firmware and establish a reproducible build;
2. verify the square-board pinout against its PCB source and physical board;
3. harden the LIN parser without changing verified protocol behavior;
4. retain a canonical LIN capture and button-mapping fixture;
5. verify MFA/Info Up, Down and Reset outputs;
6. finish automatic brightness control;
7. identify and validate KEY1/KEY2 with the target head unit;
8. determine the target CAN segment, bitrate, identifier and payload before
   enabling transmission.

Source-code changes are intentionally outside this documentation decision.
