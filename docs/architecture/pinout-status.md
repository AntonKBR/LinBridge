# LinBridge 2.0 pinout status

This document records evidence status; it does not select an unverified mapping.
All future pinout work targets only the square LinBridge 2.0 board. The pin
constants in the legacy firmware belong to an earlier experimental configuration.

## Hardware nets present in the design candidate

| Interface | PCB nets/components represented in the design | Status |
| --- | --- | --- |
| LIN | `LIN_TX`, `LIN_RX`, LIN transceiver and external LIN contact | LIN function demonstrated; exact firmware-to-PCB mapping still needs a signed verification table |
| CAN | `CAN_TX`, `CAN_RX`, TJA1050 and CAN H/L contacts | Hardware path present; communication and final mapping unverified |
| MFA/Info | Up, Down and Reset input/output paths | Intended functions known; final button-ID and output mapping evidence pending |
| KEY1/KEY2 | Separate SDA/SCL-labelled nets and two MCP4018 designators | Electrical behavior observed; exact variants, addresses and target-head-unit behavior unverified |
| CCS | Switch, Plus and Minus paths | Design paths present; GPIO selection and end-to-end behavior unverified |
| Illumination | `ILL_in` and illumination-control path | Basic illumination works; automatic control requires calibration |
| Power | 12 V input, regulator and 3.3 V domain | Bench-tested and working |

## Mapping rule

The canonical table must be derived in this order:

```text
ESP32 GPIO -> module pad -> PCB net -> component -> relay contact
           -> external function -> canonical firmware constant
```

Each row must be marked `confirmed`, `measured`, `inferred`, `conflicting` or
`unverified`, with a reference to the schematic/PCB revision and test record.

## Known conflicts requiring resolution

- The legacy firmware activates a previous LIN and output pin configuration and
  must not be used as the LinBridge 2.0 pinout authority.
- Candidate KEY1 SDA/SCL ordering must be checked against the editable PCB and
  the installed MCP4018 devices; I2C addresses are not yet known.
- Example CCS GPIO 14-17 assignments found in later experimental firmware are
  not accepted as hardware mappings without trace or continuity evidence.
- CAN bitrate, identifier and payload are experimental values, not pinout or
  protocol facts.

No canonical GPIO mapping is published until the assembled square board, PCB
source and the firmware used in the successful LIN test have been reconciled.
