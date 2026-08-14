# LinBridge research dossier

This directory reconstructs the LinBridge investigation from the surviving chat
exports, measurements, firmware history, schematics, PCB production files and
photographs. It is a technical record, not a claim that every statement made in
the chats is correct.

## Research question

Can controls from a Volkswagen MQB steering wheel be operated outside their
original vehicle network, decoded over LIN and translated into signals usable by
an older vehicle, an aftermarket head unit or a CAN/GPIO integration?

## Main result

The work progressed from an unknown five-pin button module to a bench-tested LIN
master and two assembled hardware implementations. The narrow development board
established the protocol and diagnostic workflow. The square LinBridge 2.0 board
then integrated the design into the enclosure and mounting position of the
original VAG multifunction-steering-wheel relay. LIN wake-up, polling, tested
button decoding, web identification and basic illumination control work on the
bench. CAN, automatic brightness and the target-head-unit KEY path remain open.

The strongest evidence is the executable firmware and its commit history. Chat
answers are supporting laboratory notes only; many early baud-rate, pinout and
wake-up suggestions were hypotheses and must not be treated as measurements.

## Documents

- [Research method and evidence model](01-method-and-scope.md)
- [Chronology and experiment record](02-experiment-chronology.md)
- [LIN protocol findings](03-lin-findings.md)
- [Electronics, firmware and PCB evolution](04-system-development.md)
- [Results, limitations and next experiments](05-results-and-open-work.md)
- [Hardware evolution and canonical-platform decision](06-hardware-evolution-decision.md)
- [Public source register](source-register-public.md)

The raw discussion archive and complete local source inventory are retained
privately and are not part of the public repository.

## Evidence labels

- **Observed**: reported directly from a bench measurement, trace or device output.
- **Reproduced**: implemented and retained in firmware or a production artifact.
- **Inferred**: interpretation consistent with observations but not independently verified.
- **Proposed**: an idea from a chat or design discussion that was not demonstrated.

## Safety boundary

LinBridge concerns steering-wheel electronics, but the research record does not
validate airbag compatibility or authorize modification of a restraint system.
Bench experiments and vehicle installation must keep the airbag circuit outside
the experimental electronics and require qualified review.
