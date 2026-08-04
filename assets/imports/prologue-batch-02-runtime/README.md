# CyberCBA 2077 — Batch 02 Runtime

This archive contains 20 individual game-ready PNG assets derived from the
approved Batch 1 canonical artwork.

## Canonical Magga lock

Magalí “Magga” Temerro is:

- blonde;
- long blonde/light-golden hair;
- green-eyed;
- dressed in black practical/tactical clothing.

No dark-haired Magga asset is included in this pack.

## Runtime contract

- Portraits: 1024×1280 RGBA
- Evidence: 1200×900 RGB
- Hacking background: 1920×1080 RGB
- Security drone: 1200×1200 RGBA

## Import

Extract at the repository root and load:

```text
assets/manifests/v0.1.0-prologue-batch-02-runtime.json
```

Validate:

```bash
python scripts/validate_batch_02_runtime.py
```

## Production note

Expression portraits are deterministic derivatives of the approved canonical
Batch 1 identities. This intentionally prevents character drift between
missions and keeps Magga blonde in every state.
