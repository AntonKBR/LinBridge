# LinBridge 3.0 schematic source

`source/SCH_LinBridge_3.0_2026-08-19_sanitized.json` is a public derivative of
the supplied EasyEDA 6.5.57 schematic. To avoid publishing personal identifiers,
exactly two title-block text values were replaced:

| Field | Original value | Public value |
| --- | --- | --- |
| `Drawn By` | personal development handle | `LinBridge project` |
| `Company` | personal handle | `LinBridge` |

No component, net, pin, geometry, electrical attribute, or other schematic
content was intentionally changed. The supplied original parsed as valid JSON
and had SHA-256:

```text
f4fa5773e0c8d7dade45685730a5bc1e9ffc1228cf4f97d913800e80b05a7f6f
```

The public derivative parses as valid JSON and its digest is recorded in the
revision `SHA256SUMS`. The original remains outside the public repository. This
sanitization does not modify the immutable ordered Gerber archive.
