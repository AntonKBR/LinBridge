# LinBridge 2.0 hardware

LinBridge 2.0 is the active hardware platform: a square two-layer PCB with
single-sided component assembly, designed to replace the original PCB inside a
VAG multifunction-steering-wheel relay enclosure. The earlier narrow board is a
completed development prototype and is not an active release target.

## License status

Hardware design files are published for documentation and evaluation. The final
hardware license has not yet been selected.

The hardware design must not be described as open-source until a separate
hardware license is selected. The repository-level MIT License applies to
firmware and documentation, not to these hardware design files.

## Directory contents

- `source/pcb/` contains the editable EasyEDA PCB source.
- `schematic/` contains the schematic PDF export.
- `pcb/` contains the PCB layout PDF export.
- `bom/` contains an unverified design-candidate BOM.
- `manufacturing/candidates/` contains Gerber packages that are not confirmed
  production packages.
- `manufacturing/production/` documents the acceptance conditions for a future
  production package.
- `SHA256SUMS` records integrity hashes for the published artifacts.

## Design artifact status

The editable PCB source, BOM, schematic and layout exports are dated 2025-09-01.
They describe a coherent design candidate, but have not yet been bound to a
verified physical assembly manifest.

The BOM lists two MCP4018 devices but contains an unresolved discrepancy between
`MCP4018T-103E/LT` and `MCP4018T-104E/LT`. An MCP4018 is installed; the exact
variant must be established from physical marking or supplier-order evidence.
The BOM must not be treated as a confirmed assembly manifest.

## Manufacturing candidates

### 2025-09-09 historical candidate

`manufacturing/candidates/Gerber_LinBridge_2.0_2025-09-09.zip` is the historical
candidate retained from the design archive. It contains top and bottom copper,
but intentionally omits the top-silkscreen `.GTO` file.

### 2026-08-14 canonical manufacturing candidate

`manufacturing/candidates/Gerber_LinBridge_2.0_2026-08-14.zip` is the current
canonical manufacturing candidate — not yet matched to the last supplier order.
It was exported by EasyEDA 6.5.57 and includes top and bottom copper, plated-hole
drills, outline, paste, solder mask and top and bottom silkscreen files.

SHA-256:

```text
8598ad556c318cc92d715cbddcf104788f174041a3f67bb990e6780b995cbbb6
```

Neither candidate is currently designated as a production package. That status
requires a match against the last supplier order or production preview and the
assembled square board.

## Physical validation status

The square board has been manufactured, assembled and operated on a bench.
Power, ESP32 operation, LIN wake-up and polling, tested-button identification in
the web UI and basic illumination control have been demonstrated. CAN software
is unfinished, automatic brightness needs refinement, and KEY1/KEY2 has not been
validated with the target head unit.

See the repository [validation status](../../docs/testing/validation-status.md)
and [pinout status](../../docs/architecture/pinout-status.md) for the current
evidence boundaries.
