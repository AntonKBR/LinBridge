# LIN protocol findings

## Confirmed architecture

LinBridge acts as the LIN master for a steering-wheel button module. A polling
cycle sends master frames and receives state responses. The retained firmware
uses three logical operations:

- ignition/backlight frame based on raw identifier `0x0D`;
- steering-wheel button request producing PID `0x8E`;
- ACC request based on raw identifier `0x0F`, parsed as PID `0xCF`.

The implementation computes the protected identifier parity instead of assuming
that the six-bit raw identifier and transmitted PID are interchangeable.

## Implemented timing and frame generation

Current code configures the LIN UART as `19200, SERIAL_8N1`. A break is generated
by holding TX low for 1,000 microseconds, releasing it, restarting UART and then
sending `0x55` plus PID. The backlight frame includes four data bytes and an
enhanced checksum. Request headers for button and ACC state stop after the PID so
the slave can publish its response.

Earlier chats explored 9,600, 10,417 and 19,200 bit/s, multiple wake-up pulse
durations, repeated zero bytes, `0x80` wake commands and 32 ms delays. Those are
valuable failed/proposed branches, but only the 19,200 bit/s + 1 ms break behavior
is present in the retained ESP32 implementation.

## Implemented state mapping

The current decoder contains the following mappings:

| Data value | Function |
| ---: | --- |
| `0x02` | Source + |
| `0x03` | Source - |
| `0x04` | Arrow up / MFA up |
| `0x05` | Arrow down / MFA down |
| `0x07` | OK / MFA reset |
| `0x10` | Volume + |
| `0x11` | Volume - |
| `0x15` | Next |
| `0x16` | Previous |
| `0x19` | Voice |
| `0x23` | View |
| `0x81` | ACC Set |
| `0x82` | ACC speed + |
| `0x84` | ACC speed - |
| `0x88` | ACC Resume |
| `0xB0` | ACC distance |
| `0x01`, `0x02`, `0x03` | paddle -, paddle +, both |

The GPIO layer currently exposes only a subset: MFA up/down/reset and ACC
set/speed up/speed down. The distinction between decoded and physically emitted
functions must be preserved.

## Multiple buttons and press state

The response parser tracks first and second button fields independently and
generates an event only on state change. Release is emitted when the respective
field returns to zero. ACC uses `0x80` as its neutral value. This evolution is
visible in the commits for press-state reading, second-button handling and cruise
control handling.

## Backlight

The code maps an analog input from approximately 0.5-2.1 V to brightness
`0x08-0x64`, but the transmitted ignition frame currently uses fixed `0x64`
(100 percent) rather than the calculated value. Therefore analog measurement and
mapping are implemented, while dynamic end-to-end dimming is not active in the
current source.

## Unresolved protocol questions

- The retained repository has no canonical raw analyzer capture tied to a commit.
- Receive parsing assumes byte positions but does not validate response checksum.
- The one-second transmission guard in `parseResponse` can suppress legitimate
  rapid events and conflicts with the stated fast-button goal.
- Bounds are not checked before reading response indices.
- The exact wheel/module part number and variant are not recorded in the repo.
