#!/usr/bin/env python3
"""Generates the selection-only Emma sprite with an optical scale correction and
the comparison/debug previews for the Emma vs. Magga character-selection screen.

Rationale: assets/raw/characters/emma_idle_alpha_source.png and magga_idle.png trim
to an identical alpha bounding box (136x352 inside a 256x384 canvas), so the raw
outer geometry is already equal. The perceived size mismatch comes from Emma's more
compact/crouched pose occupying less optical mass within that identical box, not
from the canvas or pivot. This script bakes a small uniform scale increase into a
*separate* selection-only asset so the shared world sprite (assets/processed/
characters/emma_idle.png), used for gameplay, is not affected.

Usage: python3 scripts/normalize_characters.py
"""
from __future__ import annotations

import json
from pathlib import Path

from PIL import Image, ImageDraw

ROOT = Path(__file__).resolve().parent.parent
RAW_EMMA = ROOT / "assets/raw/characters/emma_idle_alpha_source.png"
RAW_MAGGA = ROOT / "assets/raw/characters/magga_idle.png"
OUT_EMMA_SELECTION = ROOT / "assets/processed/characters/emma_idle_selection.png"
OUT_PREVIEW_DIR = ROOT / "assets/data/previews"
CANVAS = (256, 384)
INNER = (CANVAS[0] - 32, CANVAS[1] - 32)  # matches scripts/process_assets.sh margins
ALPHA_THRESHOLD = int(0.03 * 255)
SELECTION_SCALE_EMMA = 1.07


def alpha_bbox(im: Image.Image) -> tuple[int, int, int, int]:
    alpha = im.getchannel("A")
    mask = alpha.point(lambda p: 255 if p > ALPHA_THRESHOLD else 0)
    return mask.getbbox()


def visual_center(im: Image.Image) -> tuple[float, float]:
    import numpy as np

    a = np.array(im.getchannel("A"), dtype=np.float64)
    ys, xs = np.mgrid[0 : a.shape[0], 0 : a.shape[1]]
    total = a.sum()
    return float((xs * a).sum() / total), float((ys * a).sum() / total)


def build_selection_asset(scale: float) -> dict:
    src = Image.open(RAW_EMMA).convert("RGBA")
    bbox = alpha_bbox(src)
    cropped = src.crop(bbox)

    base_w, base_h = INNER
    ratio = min(base_w / cropped.width, base_h / cropped.height)
    target_w = round(cropped.width * ratio * scale)
    target_h = round(cropped.height * ratio * scale)
    # Clamp so the scaled content never exceeds the canvas (would clip the head).
    if target_h > CANVAS[1]:
        overflow = target_h / CANVAS[1]
        target_w, target_h = round(target_w / overflow), round(target_h / overflow)
    resized = cropped.resize((target_w, target_h), Image.NEAREST)

    # Center like scripts/process_assets.sh's `-gravity center -extent` step: the
    # baseline is not pixel-pinned (the selection screen fits the whole texture via
    # aspectFit and never reads pivot metadata), so symmetric centering keeps both
    # head and feet safely inside the canvas while maximizing the optical scale gain.
    canvas = Image.new("RGBA", CANVAS, (0, 0, 0, 0))
    x = (CANVAS[0] - target_w) // 2
    y = (CANVAS[1] - target_h) // 2
    canvas.alpha_composite(resized, (x, y))
    canvas.save(OUT_EMMA_SELECTION)

    bbox2 = alpha_bbox(canvas)
    cx, cy = visual_center(canvas)
    return {
        "canvas": {"w": CANVAS[0], "h": CANVAS[1]},
        "alphaBounds": {
            "x": bbox2[0],
            "y": bbox2[1],
            "w": bbox2[2] - bbox2[0],
            "h": bbox2[3] - bbox2[1],
        },
        "visualCenter": {"x": round(cx, 1), "y": round(cy, 1)},
        "pivot": {"x": CANVAS[0] // 2, "y": bbox2[3]},
        "selectionScale": scale,
    }


def measure(path: Path) -> dict:
    im = Image.open(path).convert("RGBA")
    bbox = alpha_bbox(im)
    cx, cy = visual_center(im)
    return {
        "canvas": {"w": im.width, "h": im.height},
        "alphaBounds": {"x": bbox[0], "y": bbox[1], "w": bbox[2] - bbox[0], "h": bbox[3] - bbox[1]},
        "visualCenter": {"x": round(cx, 1), "y": round(cy, 1)},
    }


def guides(draw: ImageDraw.ImageDraw, ox: int, oy: int, metrics: dict, label: str) -> None:
    b = metrics["alphaBounds"]
    baseline_y = oy + b["y"] + b["h"]
    head_top_y = oy + b["y"]
    eye_line_y = oy + b["y"] + round(b["h"] * 0.12)
    vc = metrics["visualCenter"]
    draw.line([(ox, baseline_y), (ox + metrics["canvas"]["w"], baseline_y)], fill=(255, 210, 90, 255), width=1)
    draw.line([(ox, head_top_y), (ox + metrics["canvas"]["w"], head_top_y)], fill=(120, 220, 255, 255), width=1)
    draw.line([(ox, eye_line_y), (ox + metrics["canvas"]["w"], eye_line_y)], fill=(255, 90, 90, 200), width=1)
    draw.ellipse([ox + vc["x"] - 3, oy + vc["y"] - 3, ox + vc["x"] + 3, oy + vc["y"] + 3], outline=(0, 255, 0, 255))
    draw.text((ox + 4, oy + 4), label, fill=(255, 255, 255, 255))


def make_previews(emma_metrics: dict, magga_metrics: dict) -> None:
    OUT_PREVIEW_DIR.mkdir(parents=True, exist_ok=True)
    emma_sel = Image.open(OUT_EMMA_SELECTION).convert("RGBA")
    emma_world = Image.open(ROOT / "assets/processed/characters/emma_idle.png").convert("RGBA")
    magga = Image.open(RAW_MAGGA if False else ROOT / "assets/processed/characters/magga_idle.png").convert("RGBA")

    def pair(a: Image.Image, b: Image.Image, pad: int = 24) -> Image.Image:
        w = a.width + b.width + pad * 3
        h = max(a.height, b.height) + pad * 2
        canvas = Image.new("RGBA", (w, h), (10, 16, 20, 255))
        canvas.alpha_composite(a, (pad, h - pad - a.height))
        canvas.alpha_composite(b, (pad * 2 + a.width, h - pad - b.height))
        return canvas

    pair(emma_sel, magga).save(OUT_PREVIEW_DIR / "characters_selection_comparison.png")
    pair(emma_world, magga).save(OUT_PREVIEW_DIR / "characters_world_scale_comparison.png")

    debug = pair(emma_sel, magga)
    draw = ImageDraw.Draw(debug)
    pad = 24
    guides(draw, pad, debug.height - pad - emma_sel.height, emma_metrics, "emma_idle_selection (x1.07)")
    guides(draw, pad * 2 + emma_sel.width, debug.height - pad - magga.height, magga_metrics, "magga_idle (x1.0)")
    debug.save(OUT_PREVIEW_DIR / "characters_debug_bounds.png")


if __name__ == "__main__":
    emma_metrics = build_selection_asset(SELECTION_SCALE_EMMA)
    magga_metrics = measure(ROOT / "assets/processed/characters/magga_idle.png")
    make_previews(emma_metrics, magga_metrics)
    print(json.dumps({"emma_idle_selection": emma_metrics, "magga_idle": magga_metrics}, indent=2))
