# Validation status

The statuses below distinguish demonstrated behavior from planned work. LinBridge
is an engineering prototype, not a production-qualified automotive control unit.

| Area | Status | Evidence boundary |
| --- | --- | --- |
| Power supply | Confirmed on bench | Assembled hardware powers and operates normally |
| ESP32 operation | Confirmed on bench | Controller and diagnostic interface run |
| LIN wake-up | Confirmed on bench | Steering-wheel module wakes |
| LIN polling | Confirmed on bench | Responses are received during master polling |
| Button decoding | Confirmed for tested controls | Web UI shows the expected tested-button identity |
| Web interface | Confirmed as bench diagnostic | Fixed credentials and unauthenticated HTTP restrict it to an isolated bench |
| Basic illumination | Confirmed on bench | Button illumination can be enabled and controlled |
| Automatic brightness | Incomplete | Input mapping and full-range behavior need calibration |
| MFA/Info Up/Down/Reset | Requires retained validation evidence | Intended hardware path exists; final mapping test remains open |
| KEY1/KEY2 | Partially observed | Resistance response checked; exact device variants, addresses and target head unit remain unverified |
| CAN | Not validated | Hardware path exists; software contract, bitrate, ID and payload are unverified |
| Production Gerber | Not confirmed | 2026 candidate has not been matched to the last supplier order |
| Automotive qualification | Not performed | EMC, ESD, load dump, reverse polarity, temperature, sleep current and functional safety are outside current evidence |

## Minimum evidence for future tests

Record the hardware revision, firmware commit, wiring, supply voltage, test
equipment, stimulus, raw capture, expected result and actual result. Public test
data must be placed only under `test-data/sanitized/` after privacy review.
