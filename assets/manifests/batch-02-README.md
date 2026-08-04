# CyberCBA 2077 — v0.1.0 Prologue Batch 02

This package converts the supplied 20-asset request into a deterministic,
reviewable generation pipeline.

## State

```text
generation-ready-not-runtime-ready
```

No asset is marked runtime-ready until an individual PNG exists at its declared
path and passes validation. Composite/contact-sheet images are stored only under
`assets/generated/previews/` and must never be imported by the game.

## Contents

- Original batch request
- One Markdown prompt per asset
- Normalized manifest
- Generation queue for Claude/Codex/manual execution
- Output directories
- Validation script
- Development previews: batch_02_composite_preview_1.png, batch_02_composite_preview_2.png, batch_02_composite_preview_3.png

## Generate

Generate one image per queue item. Never ask the generator for the complete
batch as one image because that produces a contact sheet rather than usable
runtime assets.

For each item:

1. Read its prompt file.
2. Supply the declared character consistency sheet when required.
3. Generate exactly one image.
4. Store it at `output`.
5. Resize/crop once to `targetSize`.
6. Preserve real alpha where required.
7. Review identity and embedded-text constraints.
8. Change status to `runtime-ready` only after validation.

## Validate pipeline structure

```bash
python scripts/validate_batch_02.py --allow-pending
```

## Validate final runtime files

```bash
python scripts/validate_batch_02.py
```

## Important

The built-in image generator returned composite preview sheets when the full
20-item batch was requested simultaneously. Those previews are useful for art
direction review, but they are not final game assets and are intentionally kept
outside runtime directories.
