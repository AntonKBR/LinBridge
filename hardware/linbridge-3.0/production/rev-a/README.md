# LinBridge 3.0 Rev A production package

## Release identity

| Field | Value |
| --- | --- |
| Product | LinBridge 3.0 |
| Hardware revision | Rev A |
| PCB order date | 2026-08-19 |
| Status | PCB ordered, hardware validation pending |
| Ordered archive | `Gerber_LinBridge_3.0_PCB_LinBridge_3.0_2026-08-19.zip` |
| SHA-256 | `44ac076c723f2da8008edf4604651abb0cf0def6b0971bccb245ccb546a1ec76` |

The ZIP in this directory is the unchanged package used for the actual Rev A
PCB order. Treat it as immutable release evidence. Do not regenerate it in
place, replace individual files inside it, or silently bind newer schematic or
PCB sources to Rev A. Any manufacturing change requires a new revision such as
Rev B and a new release package.

## Package scope

The archive contains top and bottom copper, solder mask and silkscreen; top
paste; board outline and document Gerbers; separate plated-through-hole and via
drill files; and EasyEDA's generic ordering instruction. It contains no BOM,
CPL/pick-and-place file, schematic, editable PCB source, PDF, assembly drawing,
or image.

Separate schematic and PCB JSON files were supplied alongside this archive. The
parent revision tree retains the PCB source exactly and a metadata-sanitized
public derivative of the schematic. Their repository hashes are recorded in the
parent `SHA256SUMS`; neither file came from inside the ordered archive.

The lack of BOM and CPL means this package proves the ordered bare-PCB geometry,
not the exact assembly population, substitutions, or DNP choices. Such files may
be added to this release only if they can be matched to the same real order
without modifying the existing archive.

## Connector constraint

The external connector footprint comes from the original VW 451/452 relay PCB.
Its intentionally nonstandard physical contact sequence must not be normalized
or reordered after the fact. Orientation and physical pin mapping require
explicit inspection during hardware bring-up.
