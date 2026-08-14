# LIN legacy behavior parity

The retained ESP32 Wi-Fi prototype is the behavioral baseline for the LIN
migration. Refactoring must preserve its observable behavior for valid frames;
structural cleanup alone is not a reason to alter working timing or button state
transitions.

## Extracted baseline

The following behavior comes directly from the retained legacy source:

| Behavior | Legacy value or rule |
| --- | --- |
| Poll order | illumination, buttons, ACC |
| Button response window | full 40 ms, even when bytes arrive earlier |
| ACC response window | full 40 ms, even when bytes arrive earlier |
| Startup output suppression | first 1,000 ms after boot |
| First button | raw frame byte 3 / data byte 1; neutral `0x00` |
| Second button | raw frame byte 4 / data byte 2; neutral `0x00` |
| ACC button | raw frame byte 4 / data byte 2; neutral `0x80` |
| Held button | one press event; repeated frames emit nothing |
| Release | neutral value releases the remembered code |
| Two button slots | tracked independently |

The legacy variable named `lastTransmitTime` was initialized to zero and never
updated. Its practical behavior was therefore a one-time startup suppression,
not a one-second debounce between events. The new tracker preserves that actual
behavior.

The legacy direct transition `A -> B` in one slot emitted `press B` without a
synthetic `release A`. This edge case is retained in the parity model and called
out explicitly so it cannot change accidentally. Whether it is desirable can be
decided only after hardware traces show how the steering wheel represents such
a transition.

The legacy output table routed button codes `04`, `05`, `07`, `81`, `82` and
`84` to GPIO outputs. Output routing is not active in the new firmware yet; this
stage preserves the upstream events that a later output adapter will consume.

## Intentional safety differences

The new implementation rejects invalid PID parity, unexpected PID, wrong
length, overflow and bad checksum before creating an event. The retained legacy
parser did not perform all of those checks. These differences affect corrupt
frames only and must not change the behavior of valid captured frames.

## Automated parity scenarios

The host parity test covers:

- startup suppression and a button still held when the gate opens;
- short press and release;
- held button without repeated events;
- simultaneous activity in both button slots;
- ACC press, hold and `0x80` release;
- the exact legacy `A -> B` direct-transition behavior;
- full 40 ms button and ACC receive windows;
- checksum rejection and timeout accounting.

These tests protect software behavior. A logic-analyzer comparison between the
legacy and diagnostic new firmware remains required to confirm break timing,
physical-bus response timing, wake-up and illumination behavior.
