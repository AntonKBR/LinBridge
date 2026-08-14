# Retained LIN research audit

This note records the useful evidence recovered from the historical LIN folder
and separates captured behavior from early experiments and conversational
hypotheses. It supplements, but does not replace, the working legacy baseline.

## Evidence hierarchy

1. Captured byte streams and the retained working firmware are behavioral
   evidence.
2. Repeated user-authored field notes attached to those captures are working
   interpretations.
3. Generated explanations, generic Arduino examples and early scan scripts are
   leads only. They must not override captured timing or checksum arithmetic.

The large HTML, DOCX and 970-page PDF are overlapping exports of the same
research conversation. Repetition between those exports is not independent
confirmation.

## Confirmed response profiles

Both known button profiles answer protected identifier `0x8E` with eight data
bytes and use enhanced checksum:

| Source/profile | Example idle response | Checksum |
| --- | --- | --- |
| Retained bench wheel `0x90` | `55 8E 10 00 00 00 90 00 00 00 D0` | enhanced (PID + data) |
| Real-car MQB/Skoda `0xA3` | `55 8E 10 00 00 00 A3 00 00 00 BD` | enhanced (PID + data) |

The historical document titled `VW MQP Platform LIN-bus Analyze` calls the
`0xA3` example a classic checksum. That statement contradicts the bytes:
classic checksum over `10 00 00 00 A3 00 00 00` is `4C`, while including PID
`8E` produces the captured `BD`. An executable regression test now protects
this correction. The validator therefore remains enhanced for button PID
`0x8E`; wheel type is data, not a checksum-profile switch.

### Button response data layout

| Data byte | Current interpretation | Confidence |
| --- | --- | --- |
| 0 | rolling sequence counter | captured and observed |
| 1 | first button code; `00` neutral | captured and used by legacy |
| 2 | second button code; `00` neutral | captured and used by legacy |
| 3 | first-button press type/duration | captured correlations |
| 4 | wheel profile (`90` bench, `A3` real-car source) | captured |
| 5 | second-button press type/duration | source field description; needs a two-button capture |
| 6 | paddles: `00` none, `01` left/- , `02` right/+ , `03` both | captured correlations |
| 7 | auxiliary state | captured values exist; exact horn semantics not proven |

Observed `0x90` press-type values include `00` neutral, `01` short press,
`04` held beyond roughly two seconds and `06` held beyond roughly four seconds.
Values `11`, `41`, `44`, `51` and `61` were observed with two-button or held
combinations. These are complete state values, not proven independent bit flags.

Useful retained test vectors:

| Action | Frame (sync through checksum) |
| --- | --- |
| idle | `55 8E 10 00 00 00 90 00 00 00 D0` |
| OK short | `55 8E 09 07 00 01 90 00 00 00 CF` |
| OK held >2 s | `55 8E 1E 07 00 04 90 00 00 00 B7` |
| OK held >4 s | `55 8E 1D 07 00 06 90 00 00 00 B6` |
| OK + right paddle | `55 8E 1C 07 00 01 90 00 02 00 BA` |

### ACC response (`0xCF`)

The retained bench captures validate with enhanced checksum. Data byte 2 is the
ACC key state: `80` neutral, `81` Set, `82` speed +, `84` speed -, `88` Resume,
and `B0` Distance. Data byte 3 was consistently `2B` in the retained examples.

| Action | Frame (sync through checksum) |
| --- | --- |
| idle | `55 CF D2 40 80 2B 00 00 00 00 71` |
| Distance | `55 CF 21 46 B0 2B 00 00 00 00 EC` |
| speed + | `55 CF 57 48 82 2B 00 00 00 00 E2` |
| Set | `55 CF 26 40 81 2B 00 00 00 00 1D` |

## Factory schedule is not the legacy timing contract

The real-car log starts with an analyzer event displayed as `80`; the first
`0x8E` frame follows 32 ms later. Subsequent `0x8E` responses occur about every
30 ms, with other identifiers interleaved (`CF`, `BA`, `8B`, `73`, `0D`, `11`).
This describes the vehicle gateway schedule. It does not invalidate the proven
legacy implementation's 1,000 us break and full 40 ms receive windows.

The `80` event is evidence of wake activity, but the export does not prove
whether it is an application byte or the analyzer's decoding of a physical LIN
wake pulse. Therefore no byte-oriented wake command is added from this source.

## Additional identifiers retained for future work

- `BA`: two-byte steering-wheel temperature/heating response. The notes decode
  temperature as byte 0 minus 50 °C and byte 1 as `FE` off / `FF` on.
- `11`: eight-byte touch-sensitive-wheel status in the vehicle capture.
- `0D`: gateway status/illumination frames with several vehicle payloads.
- `73` and `8B`: present in the factory schedule but not sufficiently decoded.

These identifiers are outside the current K1 button-communication milestone.

## Remaining hardware captures

The historical material closes the response-layout and checksum-profile gaps,
but it cannot replace these analyzer measurements:

1. Capture the physical wake waveform and decoder output together; determine
   whether the displayed `80` is a decoded wake pulse.
2. Compare legacy and diagnostic firmware break width, sync/PID spacing,
   response start and the complete 40 ms windows.
3. Record a controlled two-button sequence to verify data byte 5.
4. Record auxiliary/horn actions independently to name data byte 7 safely.
5. Capture illumination startup and steady-state behavior for the current
   hardware, rather than adopting an older `0D` payload from another profile.
