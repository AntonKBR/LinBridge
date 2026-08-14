# Method and scope

## Material examined

The reconstruction uses five independent evidence classes:

1. Chat exports: a 41,052-line PDF extraction, a 38,323-line DOCX extraction,
   a 92,370-line HTML conversation and two short summaries.
2. Structured measurement data: `Complete_LIN_Communication_Log_Analysis.csv`.
3. Git history: 41 commits across the available branches, including the retained
   development milestones from 2024-11-29 through 2025-02-11.
4. Hardware artifacts: EasyEDA/KiCad sources, schematic and PCB PDFs, BOM files,
   Gerber archives and 3D exports from 2024-12-12 through 2025-09-09.
5. Visual evidence: photographs and screenshots saved with the HTML export.

A complete inventory and the raw source archive are retained privately. The
public repository contains only reviewed research summaries and selected design
artifacts. Originals remain unchanged.

## Reconstruction rules

- Prefer a measured trace over a chat explanation.
- Prefer working firmware and commit chronology over proposed snippets.
- Preserve contradictions instead of silently choosing the most convenient value.
- Distinguish raw LIN identifiers from protected identifiers (PID).
- Treat generic automotive advice, guessed pinouts and legal commentary in early
  chats as out of scope unless supported by project evidence.
- Mark a feature as experimental if the repository contains implementation but no
  retained end-to-end trace or acceptance test.

## Known coverage limitation

The authenticated ChatGPT project export contains all 14 listed conversations,
101 pages, 928 turns and 1,862 message items. All pagination chains terminate
with `hasMore: false`. Four exceptionally large raw EasyEDA/LIN input messages
are limited to 20,000 characters by the export interface. Their metadata,
prefixes, context and all following analysis remain available. The exact four
tails can only be recovered through an official account data export. The
affected identifiers are retained only in the private archive. This byte-level
limitation must not be confused with a missing conversation or an undocumented
conclusion.

## Reproducibility standard for future experiments

Each new measurement should record date, wheel/button-module identity, supply
voltage, transceiver, controller and firmware commit, baud rate, wiring, analyzer
and sample rate, stimulus, raw capture, decoded frame, expected result and actual
result. This is the minimum needed to turn a troubleshooting conversation into a
repeatable experiment.
