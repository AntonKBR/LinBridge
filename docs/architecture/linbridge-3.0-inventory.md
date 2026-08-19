# LinBridge 3.0 hardware inventory

## Scope and evidence

This inventory records the LinBridge 3.0 artifacts supplied on 2026-08-19. It
does not designate or modify repository production files. The manufacturing ZIP
was inspected from a temporary directory outside the repository; no extracted
files were copied into the worktree.

The supplied artifacts are:

| Artifact | Role | Size (bytes) | SHA-256 |
| --- | --- | ---: | --- |
| `Gerber_LinBridge_3.0_PCB_LinBridge_3.0_2026-08-19.zip` | manufacturing archive | 72,518 | `44ac076c723f2da8008edf4604651abb0cf0def6b0971bccb245ccb546a1ec76` |
| `PCB_PCB_LinBridge_3.0_2026-08-19.json` | editable EasyEDA PCB source | 792,251 | `253bdea7c43db178a59b31b5fc8bb156d360b9f50ecf8952f416440ee691af72` |
| `SCH_LinBridge_3.0_2026-08-19.json` | editable EasyEDA schematic source | 115,860 | `f4fa5773e0c8d7dade45685730a5bc1e9ffc1228cf4f97d913800e80b05a7f6f` |

Both JSON documents parse successfully. Their metadata identifies EasyEDA
editor version 6.5.57. The PCB document contains 337 top-level shape records;
the schematic document is titled `LinBridge_3.0` and contains one sheet named
`Schema_LinBridge_3.0`.

## Manufacturing archive contents

The ZIP passes an integrity test and contains 12 regular files with no nested
directories. The uncompressed payload is 247,408 bytes.

| Class | Files | Result |
| --- | --- | --- |
| Copper | `Gerber_TopLayer.GTL`, `Gerber_BottomLayer.GBL` | present, two-layer board |
| Solder mask | `Gerber_TopSolderMaskLayer.GTS`, `Gerber_BottomSolderMaskLayer.GBS` | present on both sides |
| Silkscreen | `Gerber_TopSilkscreenLayer.GTO`, `Gerber_BottomSilkscreenLayer.GBO` | present on both sides |
| Paste | `Gerber_TopPasteMaskLayer.GTP` | top present; no bottom paste file |
| Board profile | `Gerber_BoardOutlineLayer.GKO` | present |
| Documentation | `Gerber_DocumentLayer.GDL` | present |
| Drilling | `Drill_PTH_Through.DRL`, `Drill_PTH_Through_Via.DRL` | plated holes and vias present |
| Export helper | `How-to-order-PCB.txt` | generic EasyEDA ordering link; preserve only inside the original ZIP |

The following were **not** found inside the manufacturing ZIP:

- BOM or assembly manifest;
- CPL/centroid/pick-and-place data;
- schematic or editable PCB source;
- PDF schematic or PCB render;
- board preview, assembly drawing, 3D render, or other image;
- fabrication notes, stack-up specification, impedance table, IPC netlist, or
  supplier order receipt.

Absence of bottom paste is consistent with a top-side-only assembly export, but
that interpretation must be checked against the ordered assembly configuration.
The archive alone does not prove which components, substitutions, or DNP options
were ordered.

## Comparison with existing hardware material

The repository currently separates the completed narrow prototype from
LinBridge 2.0. LinBridge 2.0 has an editable PCB JSON, schematic and PCB PDF
exports, a candidate BOM, two Gerber candidate ZIPs, integrity hashes, and
status documentation. Its manufacturing archives are explicitly candidates,
not confirmed production packages.

The supplied LinBridge 3.0 set differs in these material ways:

- it includes both editable PCB and editable schematic JSON sources, whereas
  LinBridge 2.0 has only an editable PCB source in the repository;
- it has no PDF documentation exports and no BOM;
- it has no CPL, so it is not a complete machine-assembly handoff by itself;
- its Gerber ZIP is identified by the project record as the package used for the
  ordered board, unlike the LinBridge 2.0 candidate archives;
- it includes top and bottom silkscreen and a document layer;
- it must remain a separate revision and must not replace either `prototype/`
  or `linbridge-2.0/`.

No supplied file is byte-identical to an existing tracked hardware artifact.
The unrelated untracked root file `SCH_MP1584-3.3V_restored_0805.json` was not
used or modified during this inventory.

## Proposed LinBridge 3.0 layout

The next stage should add a revision-specific tree without changing the Legacy
or LinBridge 2.0 trees:

```text
hardware/linbridge-3.0/
├── README.md
├── SHA256SUMS
├── schematic/
│   └── source/
│       └── SCH_LinBridge_3.0_2026-08-19.json
├── pcb/
│   └── source/
│       └── PCB_LinBridge_3.0_2026-08-19.json
├── production/
│   └── rev-a/
│       ├── README.md
│       └── Gerber_LinBridge_3.0_PCB_LinBridge_3.0_2026-08-19.zip
├── docs/
└── images/
```

`production/rev-a/README.md` should bind the ZIP hash to the actual order date,
board name, hardware revision, order status, supplier/order evidence if it can
be published, and any assembly-data limitations. Generated PDF and image
exports may be added later for review, but should be clearly marked as derived
artifacts and must not be substituted into the frozen Rev A package.

A future BOM or CPL belongs in `production/rev-a/` only if it can be matched to
the actual order. A newly generated BOM or CPL is design documentation, not
evidence of what was manufactured. Any post-order electrical or layout change
must be a new revision such as Rev B.

## Publication and retention policy

Suitable for the public repository, subject to the still-unresolved hardware
license and a final check for supplier or personal metadata:

- revision README files, inventory and validation documentation;
- editable EasyEDA schematic and PCB JSON sources;
- selected PDF exports and review images generated from the versioned sources;
- `SHA256SUMS` and a concise production-package manifest;
- the exact Gerber ZIP if publishing fabrication data is intended.

Keep as an immutable release package:

- the original ordered Rev A Gerber ZIP, byte for byte;
- its recorded SHA-256 digest and order/revision manifest;
- only BOM, CPL, assembly options, substitutions, and supplier previews that can
  be proven to belong to that same order.

Do not add extracted duplicates of the Gerbers alongside the ZIP, the generic
`How-to-order-PCB.txt` as a standalone repository file, temporary extraction
directories, editor caches, private ChatGPT exports, personal data, or supplier
credentials. Private invoices and order records may be retained outside the
public repository; a sanitized order identifier or checksum can be recorded in
the release manifest when useful.

Publishing files in this repository does not by itself make the hardware open
source. The repository currently states that the final hardware license has not
been selected, so LinBridge 3.0 documentation must retain that qualification.
