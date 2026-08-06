#!/usr/bin/env python3
from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path
from PIL import Image

ROOT = Path(__file__).resolve().parents[1]
MANIFEST_PATH = ROOT / "assets/manifests/v0.1.0-prologue-batch-02.json"

def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--allow-pending",
        action="store_true",
        help="Validate the pipeline structure without requiring generated files.",
    )
    args = parser.parse_args()

    manifest = json.loads(MANIFEST_PATH.read_text(encoding="utf-8"))
    errors = []
    warnings = []
    ids = set()

    for asset in manifest["assets"]:
        asset_id = asset["id"]
        if asset_id in ids:
            errors.append(f"Duplicate asset id: {asset_id}")
        ids.add(asset_id)

        prompt_path = ROOT / asset["promptPath"]
        if not prompt_path.exists():
            errors.append(f"{asset_id}: missing prompt file {asset['promptPath']}")

        path = ROOT / asset["path"]
        if not path.exists():
            message = f"{asset_id}: pending file {asset['path']}"
            if args.allow_pending:
                warnings.append(message)
                continue
            errors.append(message)
            continue

        expected = asset["target"]
        with Image.open(path) as img:
            if img.size != (expected["width"], expected["height"]):
                errors.append(
                    f"{asset_id}: expected {expected['width']}x{expected['height']}, "
                    f"got {img.width}x{img.height}"
                )

            has_alpha = "A" in img.mode
            if expected["alphaRequired"] and not has_alpha:
                errors.append(f"{asset_id}: alpha channel required")
            if expected["alphaRequired"] and has_alpha:
                lo, hi = img.getchannel("A").getextrema()
                if lo == 255:
                    errors.append(f"{asset_id}: alpha channel exists but image is fully opaque")
                if hi == 0:
                    errors.append(f"{asset_id}: image is fully transparent")

    for warning in warnings:
        print(f"WARNING: {warning}")

    if errors:
        print("Validation failed:")
        for error in errors:
            print(f" - {error}")
        return 1

    print(
        f"Validation passed for {len(manifest['assets'])} assets "
        f"({'pending allowed' if args.allow_pending else 'runtime mode'})."
    )
    return 0

if __name__ == "__main__":
    sys.exit(main())
