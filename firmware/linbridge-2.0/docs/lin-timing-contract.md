# LIN timing and frame contract

This contract preserves the parameters reproduced by the retained ESP32
prototype. The implementation is compiled but not invoked at this migration
stage; receive validation and scheduling will be added before LIN is enabled.

| Parameter | Value | Status |
| --- | ---: | --- |
| UART | 19,200 baud, 8N1 | reproduced; bench LIN path confirmed |
| Break duration | 1,000 microseconds dominant | reproduced; distribution unverified |
| Sync byte | `0x55` | reproduced |
| Illumination raw identifier / PID | `0x0D` / `0x0D` | reproduced |
| Button raw identifier / PID | `0x0E` / `0x8E` | reproduced |
| ACC raw identifier / PID | `0x0F` / `0xCF` | reproduced |
| Button response | PID `0x8E`, 8 data bytes, enhanced checksum | confirmed by retained bench captures |
| ACC response | PID `0xCF`, 8 data bytes, enhanced checksum | confirmed by retained bench captures |
| Response window | 40 milliseconds | reproduced; final timing margin unverified |

The illumination frame retains the fixed payload `64 FF FF FF` and enhanced
checksum `8E`. The calculated ADC brightness from the legacy source is not
carried forward because the transmitted legacy frame used the fixed value.

## Preserved polling order

1. Send the fixed illumination frame.
2. Send the button request header and open a response window.
3. Validate and decode the button response.
4. Send the ACC request header and open a response window.
5. Validate and decode the ACC response.

The receive transport captures at most 16 bytes and records overflow without
writing outside the buffer. The button and ACC profiles are selected from
retained full-frame bench captures. Both responses contain 8 data bytes and use
the enhanced checksum model.

## Evidence-backed response fields

Data offsets below exclude the sync byte and PID.

| Response | Data byte | Meaning retained for this stage |
| --- | ---: | --- |
| Button (`0x8E`) | 0 | rolling sequence counter |
| Button (`0x8E`) | 1 | button code; `0x00` is neutral |
| ACC (`0xCF`) | 0 | unknown; values vary between otherwise equivalent captures |
| ACC (`0xCF`) | 1 | rolling sequence counter (`0x40` through `0x4F` observed) |
| ACC (`0xCF`) | 2 | button state; `0x80` is neutral |
| ACC (`0xCF`) | 3 | control state; `0x2B` accompanies neutral captures |

The decoder exposes only these established fields. In particular, ACC data byte
0 is deliberately named `unknown0`, and no complete semantic button mapping is
claimed at this stage. The private source archive remains outside the repository;
tests contain only minimal hexadecimal protocol vectors.

Polling remains disabled until the receive transport, scheduler and application
event mapping are connected in later stages. The response decoder rejects every
frame for which validation does not return `kNone`.
