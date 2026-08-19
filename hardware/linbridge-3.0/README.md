# LinBridge 3.0 hardware

LinBridge 3.0 is the current hardware revision of the square
relay-replacement board.

| Field | Value |
| --- | --- |
| Product name | LinBridge 3.0 |
| Hardware revision | Rev A |
| Order date | 2026-08-19 |
| Status | PCB ordered, hardware validation pending |
| Design tool | EasyEDA 6.5.57 |

## Revision identity

Rev A is the board represented by the frozen Gerber archive in
`production/rev-a/`. The archive is the package used for the actual PCB order;
its SHA-256 digest is recorded in `SHA256SUMS` and in the production manifest.
The editable PCB JSON is retained byte for byte as a corresponding design
source. The public schematic JSON is a controlled derivative with only personal
author and company strings replaced; its electrical content is otherwise
unchanged. Neither JSON document is a member of the manufacturing ZIP.

Do not modify the ordered Rev A production package retrospectively. Electrical,
layout, footprint, or fabrication corrections made after the order must be
released as a new hardware revision, for example Rev B.

## VW connector footprint

The external connector footprint was derived from the original VW 451/452
relay PCB. Its physical contact sequence is nonstandard and intentionally
reproduces the original board. Do not renumber or reorder footprint pads merely
to make their physical order sequential. Schematic and PCB sources are the
source of truth for connectivity; the complete external pin table will be
documented and checked directly against both sources in the next documentation
stage.

The footprint carries orientation risk. Assembly and bring-up must explicitly
check the board side, mating direction, pin-1 reference, and the mapping between
physical contacts and schematic numbers before the board is connected to a
vehicle.

## Directory contents

- `schematic/source/` contains the public, metadata-sanitized editable EasyEDA
  schematic JSON and a sanitization record.
- `pcb/source/` contains the original editable EasyEDA PCB JSON.
- `production/rev-a/` contains the immutable ordered Gerber ZIP and its
  production manifest.
- `docs/` is reserved for revision-specific technical and validation documents.
- `images/` is reserved for reviewed renders and photographs.
- `SHA256SUMS` records integrity hashes for the exact PCB source and production
  ZIP and for the public schematic derivative.

No BOM or CPL/pick-and-place file was supplied with the ordered Gerber archive.
A later-generated BOM or CPL must not be described as part of the actual Rev A
order unless independent order evidence confirms the match.

## License status

Hardware design files are published for documentation and evaluation. The final
hardware license has not yet been selected. The repository-level MIT License
applies to firmware and documentation, not automatically to these hardware
design files.
