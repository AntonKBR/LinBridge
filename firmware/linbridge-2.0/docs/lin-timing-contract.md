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

This step implements the transmit primitives only. It deliberately does not
approximate the missing receive validation or activate a partial polling loop.
The next LIN step must add bounded receive handling, length checks, PID and
checksum validation before composing this order in `main.cpp`.
