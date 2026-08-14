# Results, limitations and next work

## What the research demonstrated

1. Passive monitoring alone was insufficient because the button module required
   LIN master polling.
2. A master header and timing implementation could be reproduced on a bench.
3. A usable button response was received and the `0x8E` path was decoded into
   individual controls.
4. ACC, paddle and two-button cases were added through iterative firmware work.
5. The solution was migrated to ESP32 and expanded with GPIO, CAN, backlight and
   Wi-Fi diagnostic concepts.
6. A narrow two-layer development PCB validated the initial system.
7. A square two-layer LinBridge 2.0 PCB with single-sided component assembly was
   assembled and operated in the form factor of the original VAG relay enclosure.
8. LIN wake-up, polling, tested button identification in the web UI and basic
   button-illumination control work on physical hardware.

## What has not yet been scientifically demonstrated

- a repeatable analyzer capture bundle with raw signal, decoder settings and
  firmware commit for every claimed LIN frame;
- measured timing distributions across cold start, steady state and repeated
  button events;
- checksum validation of received frames and quantified error rate;
- verified CAN IDs/payloads against a defined target ECU;
- calibrated automatic backlight behavior across the full input range;
- automotive electrical qualification (reverse polarity, load dump, ESD, EMC,
  temperature and sleep-current behavior);
- safe, approved in-vehicle integration involving the steering wheel/airbag.

## Recommended next experiment set

### E1 - canonical LIN capture

Record power-up, wake/break, `0x0D`, `0x8E` and `0xCF` cycles with the logic
analyzer. Store the raw capture, screenshot, decoder configuration, supply
voltage and firmware commit. Repeat with no button, every single button, two
simultaneous buttons, paddles and each ACC control.

### E2 - timing characterization

For each cycle measure break width, sync-byte bit time, header-to-response delay,
frame period, jitter and timeout margin. Report minimum, median, maximum and
sample count rather than a single selected value.

### E3 - decoder integrity

Add length checks, PID validation, classic/enhanced response checksum validation,
error counters and raw-frame logging. Prove that malformed/truncated frames do
not produce GPIO events.

### E4 - output validation

Validate MFA/Info Up, Down and Reset mapping and record the corresponding LIN
button IDs. Determine the installed digital-potentiometer variants and I2C
addresses, then test KEY1/KEY2 with the target head unit. For CAN, first capture
and define the target contract, then test on an isolated bench. For automatic
backlight control, sweep the input while recording the transmitted value and
visible output.

### E5 - hardware release

Compare the 2026-08-14 canonical Gerber candidate with the Gerber retained in
the last supplier order and with the assembled board,
verify BOM footprints and manufacturer part numbers, run ERC/DRC and create a
signed release manifest tying schematic, PCB, BOM, Gerbers and firmware together.

## Publication status

The project has a strong reverse-engineering story, two assembled hardware
implementations and substantial evidence of iterative engineering. It is not a
production-qualified automotive product. For a public portfolio, present the
narrow board as the completed proof of concept and LinBridge 2.0 as the active,
bench-tested OEM-style integration platform, while stating the open CAN,
automatic-brightness and KEY validation work.
