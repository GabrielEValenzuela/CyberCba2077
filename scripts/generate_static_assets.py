#!/usr/bin/env python3
"""Deterministic procedural generator for reusable static UI/evidence assets.

Rationale: `docs/static-art-direction.md` and ADR 0013 list three evidence
images (`insignia`, `unsent_message`, `train41_record`) as a real, admitted
gap — `EvidenceJournal` registers those items (src/domain/PrologueContent.cpp)
but `GameApp` only shows them as text because no `processed` asset exists.
This script fills that specific gap with original, low-detail geometric
placeholders (silhouettes/photographs-as-shapes, no embedded text per the
"no localized text in generated assets" rule) rather than leaving it as a
TODO indefinitely. It does not attempt full illustration-quality art.

Usage:
    python3 scripts/generate_static_assets.py --evidence
    python3 scripts/generate_static_assets.py --all
    python3 scripts/generate_static_assets.py --validate
"""
from __future__ import annotations

import argparse
import json
import random
from pathlib import Path

from PIL import Image, ImageDraw, ImageFilter

ROOT = Path(__file__).resolve().parent.parent
OUT_DIR = ROOT / "assets/processed/evidence"
CANVAS = (128, 96)

# `cybercba_handheld` palette, matching docs/static-art-direction.md role
# allocation (cyan = network/Emma, amber = danger/Magga, gray = neutral).
NEAR_BLACK = (10, 14, 18, 255)
DARK_BLUEGRAY = (26, 38, 48, 255)
MUTED_CYAN = (94, 196, 214, 255)
WARM_AMBER = (214, 150, 74, 255)
LIGHT_GRAY = (150, 164, 172, 255)
OFF_WHITE = (226, 232, 232, 255)


def _seeded(name: str) -> random.Random:
    # Deterministic per-asset seed so re-running the generator is a no-op diff.
    return random.Random(f"cybercba-static-asset:{name}")


def _grain(im: Image.Image, rng: random.Random, amount: int = 10) -> Image.Image:
    px = im.load()
    for y in range(im.height):
        for x in range(im.width):
            r, g, b, a = px[x, y]
            if a == 0:
                continue
            n = rng.randint(-amount, amount)
            px[x, y] = (max(0, min(255, r + n)), max(0, min(255, g + n)), max(0, min(255, b + n)), a)
    return im


MARGIN = 6  # keeps corners transparent, matching the alpha-canvas convention
# validated by scripts/validate_assets.sh across all other processed assets.


def _base(name: str) -> tuple[Image.Image, ImageDraw.ImageDraw, random.Random]:
    im = Image.new("RGBA", CANVAS, (0, 0, 0, 0))
    draw = ImageDraw.Draw(im)
    inset = [MARGIN, MARGIN, CANVAS[0] - 1 - MARGIN, CANVAS[1] - 1 - MARGIN]
    draw.rectangle(inset, fill=DARK_BLUEGRAY)
    draw.rectangle(inset, outline=LIGHT_GRAY, width=2)
    return im, draw, _seeded(name)


def gen_insignia() -> Image.Image:
    """The Fireflies' insignia: a firefly silhouette over a circular badge."""
    im, draw, rng = _base("insignia")
    cx, cy, r = CANVAS[0] // 2, CANVAS[1] // 2, 34
    draw.ellipse([cx - r, cy - r, cx + r, cy + r], outline=WARM_AMBER, width=3)
    draw.ellipse([cx - r + 6, cy - r + 6, cx + r - 6, cy + r - 6], outline=LIGHT_GRAY, width=1)
    # Body
    draw.ellipse([cx - 6, cy - 4, cx + 6, cy + 16], fill=NEAR_BLACK, outline=WARM_AMBER)
    # Wings
    draw.polygon([(cx - 4, cy), (cx - 22, cy - 14), (cx - 4, cy - 10)], outline=WARM_AMBER)
    draw.polygon([(cx + 4, cy), (cx + 22, cy - 14), (cx + 4, cy - 10)], outline=WARM_AMBER)
    # Glow dot
    draw.ellipse([cx - 3, cy + 10, cx + 3, cy + 16], fill=WARM_AMBER)
    im = im.filter(ImageFilter.SMOOTH_MORE)
    return _grain(im, rng, 6)


def gen_unsent_message() -> Image.Image:
    """An unsent message draft: a folded letter silhouette on a dark desk."""
    im, draw, rng = _base("unsent_message")
    px, py, pw, ph = 24, 20, 80, 56
    draw.rectangle([px, py, px + pw, py + ph], fill=OFF_WHITE, outline=LIGHT_GRAY, width=2)
    draw.line([(px, py), (px + pw // 2, py + ph // 2), (px + pw, py)], fill=LIGHT_GRAY, width=2)
    for i in range(4):
        y = py + 14 + i * 8
        draw.line([(px + 8, y), (px + pw - 8, y)], fill=DARK_BLUEGRAY, width=2)
    # Cursor / unsent marker, no literal text per generated-asset rules.
    draw.rectangle([px + pw - 20, py + ph - 16, px + pw - 8, py + ph - 10], fill=MUTED_CYAN)
    return _grain(im, rng, 5)


def gen_train41_record() -> Image.Image:
    """Train 41 official record: a train silhouette with a corrupted overlay."""
    im, draw, rng = _base("train41_record")
    bx, by, bw, bh = 14, 50, 100, 24
    draw.rounded_rectangle([bx, by, bx + bw, by + bh], radius=6, fill=NEAR_BLACK, outline=MUTED_CYAN, width=2)
    for i in range(5):
        wx = bx + 10 + i * 18
        draw.rectangle([wx, by + 5, wx + 10, by + 13], fill=MUTED_CYAN)
    for i in range(2):
        cx = bx + 20 + i * 60
        draw.ellipse([cx - 6, by + bh - 4, cx + 6, by + bh + 8], fill=LIGHT_GRAY)
    # Corrupted-record scanline overlay (procedural effect, not embedded text).
    for _ in range(14):
        y = rng.randint(10, CANVAS[1] - 10)
        x0 = rng.randint(0, CANVAS[0] // 2)
        x1 = x0 + rng.randint(10, 50)
        draw.line([(x0, y), (x1, y)], fill=WARM_AMBER, width=1)
    return _grain(im, rng, 8)


EVIDENCE_GENERATORS = {
    "insignia": gen_insignia,
    "unsent_message": gen_unsent_message,
    "train41_record": gen_train41_record,
}


def generate_evidence() -> dict:
    OUT_DIR.mkdir(parents=True, exist_ok=True)
    results = {}
    for evidence_id, fn in EVIDENCE_GENERATORS.items():
        im = fn()
        out_path = OUT_DIR / f"{evidence_id}.png"
        im.save(out_path)
        results[evidence_id] = {"path": str(out_path.relative_to(ROOT)), "width": im.width, "height": im.height}
    return results


def validate() -> None:
    for evidence_id in EVIDENCE_GENERATORS:
        path = OUT_DIR / f"{evidence_id}.png"
        if not path.exists():
            raise SystemExit(f"missing generated asset: {path}")
        im = Image.open(path)
        if im.size != CANVAS:
            raise SystemExit(f"unexpected canvas for {path}: {im.size}, expected {CANVAS}")
        if im.mode != "RGBA":
            raise SystemExit(f"expected RGBA alpha channel for {path}, got {im.mode}")
    print(f"OK: {len(EVIDENCE_GENERATORS)} evidence assets validated at {CANVAS[0]}x{CANVAS[1]}.")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--evidence", action="store_true", help="Generate the prologue evidence image gap.")
    parser.add_argument("--all", action="store_true", help="Generate all supported categories (currently: evidence).")
    parser.add_argument("--validate", action="store_true", help="Validate previously generated assets.")
    args = parser.parse_args()

    if args.validate:
        validate()
        return
    if args.evidence or args.all:
        results = generate_evidence()
        print(json.dumps(results, indent=2))
        return
    parser.print_help()


if __name__ == "__main__":
    main()
