# LinBridge 2.0 pinout evidence

This file defines the intended ESP32-S3-Zero signal mapping for the square
LinBridge 2.0 PCB. It does not claim that every interface has passed an
end-to-end hardware test.

## Evidence boundary

The mapping was visually reconciled on 2026-08-14 against the enlarged U1
section of the author's current EasyEDA `Schema_LinBridge_2.0` design and the
newer local PlatformIO firmware candidate. The project author identified this
schematic as the correct pinout.

`Configuration status` describes agreement between the intended schematic and
firmware configuration. `Physical status` describes retained measurement or
end-to-end evidence for the assembled square board.

| Function | Schematic net | GPIO | Configuration status | Physical status |
| --- | --- | ---: | --- | --- |
| LIN transmit | `LIN_TX` | 1 | confirmed | confirmed as part of the working LIN path; per-net continuity unverified |
| LIN receive | `LIN_RX` | 2 | confirmed | confirmed as part of the working LIN path; per-net continuity unverified |
| KEY1 data | `SDA_KEY1` | 3 | confirmed | unverified with target head unit |
| KEY1 clock | `SCL_KEY1` | 4 | confirmed | unverified with target head unit |
| CCS switch input | `CCS_switch_in` | 5 | confirmed | unverified |
| Illumination measurement | `ILL_mod` | 6 | confirmed | basic illumination confirmed; automatic regulation unverified |
| CAN receive | `CAN_RX` | 7 | confirmed | unverified |
| CAN transmit | `CAN_TX` | 8 | confirmed | unverified |
| MFA reset input | `MFA_reset_in` | 9 | confirmed | final MFA mapping unverified |
| MFA down input | `MFA_key_down_in` | 10 | confirmed | final MFA mapping unverified |
| MFA up input | `MFA_key_up_in` | 11 | confirmed | final MFA mapping unverified |
| CCS minus input | `CCS-_in` | 12 | confirmed | unverified |
| CCS plus input | `CCS+_in` | 13 | confirmed | unverified |

## Explicit exclusions

- GPIO14-GPIO17 assignments from experimental firmware are not present in the
  correct schematic and are not part of the canonical board configuration.
- KEY2 and a second MCP4018 channel are not implemented or connected. Initial
  head-unit development targets KEY1 only.
- The MCP4018 variant, I2C address, wiper values and target-head-unit behavior
  remain unverified.
- No dedicated LIN `NSLP`/sleep-control GPIO is present in the accepted U1
  mapping. The legacy `NSLP_PIN` constant is not carried forward.
- USB is the programming and host-diagnostics interface. It does not consume
  GPIO1 or GPIO2 in this board configuration.
- CAN bitrate, identifier and payload are protocol configuration, not pinout
  facts, and remain unspecified.
- LIN, KEY, CCS, MFA, CAN, illumination and Wi-Fi runtime features remain
  disabled at this migration stage.
