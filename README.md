# LinBridge

LinBridge is an embedded reverse-engineering project that operates Volkswagen
MQB multifunction steering-wheel controls as a LIN master and translates their
events for use in an older vehicle. The project combines ESP32 firmware, LIN
protocol research, legacy MFA/Info outputs, experimental resistive and CAN
interfaces, a web-based bench monitor and custom electronics.

## Current status

The LIN path is bench-tested on assembled hardware:

- the steering-wheel module powers up and wakes correctly;
- the ESP32 polls the relevant LIN frames;
- tested button events are decoded and identified correctly in the web UI;
- button illumination can be enabled and controlled;
- the power supply and core electronics operate on the bench.

LinBridge is an engineering prototype, not a production-qualified automotive
control unit. CAN communication, automatic brightness control and the
KEY1/KEY2 path still require development or end-to-end validation.

## Hardware evolution

### Development board

The first implementation was a narrow, two-layer board used to establish LIN
communication, timing, wake-up, button decoding, illumination control and the
web diagnostic workflow. It remains a working proof of concept but will not
receive further development.

### LinBridge 2.0

LinBridge 2.0 is the active hardware platform. It is a square, two-layer
relay-replacement PCB with single-sided component assembly, designed for the
original VAG multifunction-steering-wheel relay enclosure. The original relay
PCB is removed, its contact pins are
transferred to LinBridge 2.0, and the module returns to the factory mounting
location after the required wiring changes. This preserves an OEM-style
installation instead of adding a visibly separate control box.

Both hardware implementations have been assembled, and their role in the LIN
development has been demonstrated. This does not imply validation of every
interface. All future firmware, pinout verification, testing and releases target
the square LinBridge 2.0 PCB.

## Feature status

| Area | Status |
| --- | --- |
| Power supply | Bench-tested and working |
| LIN master, wake-up and polling | Bench-tested and working |
| Button decoding and web identification | Bench-tested and working |
| Button illumination | Basic control works; automatic brightness needs refinement |
| MFA/Info Up, Down and Reset | Hardware path exists; final mapping evidence should be recorded |
| KEY1/KEY2 resistive output | Electrical response checked with a multimeter; target head unit not tested |
| CAN | Hardware exists; bitrate, identifier, payload and software contract are unverified |

## Documentation

- [Firmware status](firmware/README.md)
- [Research dossier](docs/research/README.md)
- [Experiment chronology](docs/research/02-experiment-chronology.md)
- [LIN findings](docs/research/03-lin-findings.md)
- [System and PCB evolution](docs/research/04-system-development.md)
- [Verified status and open work](docs/research/05-results-and-open-work.md)
- [Hardware evolution decision](docs/research/06-hardware-evolution-decision.md)
- [Architecture and pinout status](docs/architecture/pinout-status.md)
- [Validation status](docs/testing/validation-status.md)
- [LinBridge 2.0 hardware archive](hardware/linbridge-2.0/README.md)

The raw research source archive is retained locally for traceability and is
intentionally excluded from the public repository.

## Licensing

Firmware and documentation are licensed under the [MIT License](LICENSE).

Hardware design files are published for documentation and evaluation. The final
hardware license has not yet been selected. The hardware must not be described
as open-source until a separate hardware license is selected.

Project author: Anton Danilov.

## Safety boundary

The project does not validate airbag compatibility, automotive EMC, load-dump,
ESD, reverse-polarity or functional-safety compliance. Steering-wheel and
vehicle experiments must keep restraint-system circuits outside the experimental
electronics and be reviewed by a qualified person.
