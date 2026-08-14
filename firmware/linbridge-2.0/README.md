# LinBridge 2.0 firmware

This directory contains the PlatformIO scaffold for the future canonical
LinBridge 2.0 firmware. It targets only the square, two-layer LinBridge 2.0 PCB.
The historical narrow prototype remains under `firmware/legacy/` and is not a
supported target of this project.

## Current migration stage

The build foundation, board configuration and dormant LIN transmit primitives
are present. LIN receive handling and polling are not active. No output routing,
CAN, KEY1/KEY2, CCS, automatic illumination, Wi-Fi or web behavior has been
migrated. A successful build is not evidence that a hardware function has been
validated.

All runtime features are disabled in `include/linbridge/feature_flags.h`. They
must be enabled only by a later migration stage with the required evidence and
tests.

The intended GPIO1-GPIO13 mapping is defined in
`include/linbridge/board_config.h`. Its evidence boundaries and explicit
exclusions are recorded in [`docs/pinout-evidence.md`](docs/pinout-evidence.md).
The reproduced LIN constants and polling order are recorded in
[`docs/lin-timing-contract.md`](docs/lin-timing-contract.md).

## Build

PlatformIO Core 6.1 or later is required. From this directory run:

```sh
pio run
```

The Espressif32 platform version is pinned in `platformio.ini`. The provisional
`esp32-s3-devkitc-1` board definition is used only to compile for the same MCU
family as the ESP32-S3-Zero module. It does not define or validate the canonical
LinBridge 2.0 GPIO mapping.

No upload target or upload procedure is defined at this stage.
