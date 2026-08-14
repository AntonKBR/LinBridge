# Experiment chronology

## Phase 1 - problem definition and bench preparation

The initial goal was to install a Golf 7/MQB multifunction steering wheel in a
Golf 4 context and make its buttons useful to older or aftermarket equipment.
Possible outputs considered in the chats were a resistive/analog KEY1/KEY2
interface, GPIO emulation and CAN translation.

The early bench used a powered steering-wheel button block, a LIN-to-TTL adapter,
Arduino-class boards, USB-UART equipment and later STM32/ST-Link and logic-analysis
tools. One recorded multimeter observation for the five-pin block was:

| Pin | Recorded bench value | Interpretation at that time |
| --- | ---: | --- |
| 1 | ground | confirmed connection in the experiment |
| 2 | 12 V input | confirmed supply connection |
| 3 | 10.85 V | candidate LIN line |
| 4 | 11.15 V | unresolved |
| 5 | 5.7 V, slowly rising | unresolved |

Only pins 1 and 2 were explicitly connected by the experimenter. The remaining
interpretations were hypotheses; early chat pinout tables contradicted one
another and are not accepted as authoritative.

## Phase 2 - discovering the master/slave behavior

Passive listening initially produced no useful button traffic. This led to the
key architectural correction: the button block behaves as a LIN slave and does
not publish button states without a master header. Experiments then moved to:

1. generating a dominant break/wake signal;
2. transmitting sync `0x55`;
3. scanning identifiers;
4. listening for a slave response;
5. adjusting frame structure, checksum handling and delays.

The CSV scan log records alternating apparent response/no-response rows and also
contains duplicated/misaligned entries. It is evidence of the exploratory scan,
not a clean identification table. It must not be used alone to assert that every
even identifier responded.

## Phase 3 - successful communication and decoding

Git history provides the clearest milestones:

| Date | Commit | Result |
| --- | --- | --- |
| 2024-11-29 | `d675f5b` | first retained "Response Received" milestone |
| 2024-12-09 | `58e8bb0` | listener focused on frame/PID `0x8E` |
| 2024-12-09 | `59bb343` | button press listener |
| 2024-12-09 | `d700a7d` | button mapping added |
| 2024-12-11 | `65be62d` | ACC event listener added |
| 2024-12-11 | `7a99c08` | paddle/shifter decoding corrected |
| 2024-12-12 | `e5d96d6` | press-state handling retained |

The progression is important: identifier scanning and checksum experiments did
not themselves prove a working decoder; the later response, mapping and event
commits do.

## Phase 4 - ESP32 migration and timing reproduction

The project moved from the original Arduino-oriented implementation to ESP32.
On 2025-01-01 the history records successful ESP32 adjustment, a dedicated frame
timing fix and response parser correction. The current implementation uses UART
at 19,200 bit/s and generates a dominant break of 1,000 microseconds before
reinitializing UART and sending the header. These are reproduced implementation
parameters, not proof that every earlier trial used the same values.

Timeouts were subsequently reduced to capture fast button presses. This shows
that correct byte content was not sufficient: request cadence and receive-window
behavior were part of the successful emulation.

## Phase 5 - output functions and integration

Later development added:

- CAN library experiments;
- analog backlight input and LIN brightness translation;
- GPIO emulation for MFA controls;
- separate press/release state handling;
- simultaneous second-button support;
- cruise-control button handling;
- a small Wi-Fi status page for bench diagnostics.

The final two commits on the current branch expose button events over a local
ESP32 access point. This is a diagnostic aid, not a production security design.

## Phase 6 - custom electronics

Hardware work began in December 2024 with KiCad placeholders and then moved
through EasyEDA schematic, PCB, BOM and Gerber revisions. Archived milestones
exist for 2024-12-16, 12-18, 12-22/23, 12-29, 2025-01-04/06, 01-28, 08-31,
09-01, 09-02 and 09-09. The repository package under `hardware/linbridge-2.0`
preserves the latest editable 2025-09-01 design and the later 2025-09-09 Gerber
set, with their mismatch documented rather than hidden.

## Phase 7 - two working hardware implementations

The physical project produced two assembled boards with distinct roles:

1. A narrow two-layer development board validated the electrical concept, LIN
   master communication, wheel wake-up, button decoding, web identification and
   illumination control. It is a completed proof-of-concept platform.
2. The square two-layer LinBridge 2.0 board, with components assembled on one
   side, was designed as a replacement for the PCB inside the original VAG
   multifunction-steering-wheel relay. The
   original contact pins are transferred to the new PCB, allowing the module to
   return to the factory enclosure and mounting position after wiring changes.

Both boards are assembled and functional. The narrow board is no longer an
active development target. LinBridge 2.0 is the canonical platform for future
firmware, hardware validation and vehicle integration.
