#!/usr/bin/env python3
from __future__ import annotations

import hashlib
import json
import sys
from pathlib import Path
from PIL import Image

ROOT = Path(__file__).resolve().parents[1]
MANIFEST = ROOT / "assets/manifests/v0.1.0-prologue-runtime.json"

def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()

def main() -> int:
    data = json.loads(MANIFEST.read_text(encoding="utf-8"))
    ids = set()
    errors = []

    for asset in data["assets"]:
        asset_id = asset["id"]
        if asset_id in ids:
            errors.append(f"Duplicate asset id: {asset_id}")
        ids.add(asset_id)

        path = ROOT / asset["path"]
        if not path.exists():
            errors.append(f"Missing file: {asset['path']}")
            continue

        with Image.open(path) as img:
            if img.size != (asset["width"], asset["height"]):
                errors.append(
                    f"{asset_id}: expected {asset['width']}x{asset['height']}, got {img.width}x{img.height}"
                )
            has_alpha = "A" in img.mode
            if asset["alphaRequired"] and not has_alpha:
                errors.append(f"{asset_id}: alpha channel required")
            if asset["alphaRequired"] and has_alpha:
                lo, hi = img.getchannel("A").getextrema()
                if lo == 255:
                    errors.append(f"{asset_id}: alpha exists but image is fully opaque")
                if hi == 0:
                    errors.append(f"{asset_id}: image is fully transparent")

        actual_hash = sha256_file(path)
        if actual_hash != asset["sha256"]:
            errors.append(f"{asset_id}: SHA-256 mismatch")

        if asset.get("runtimeEligible", False):
            if asset.get("status") != "runtime-ready":
                errors.append(f"{asset_id}: invalid runtime status")

    if errors:
        print("Asset validation failed:")
        for error in errors:
            print(f" - {error}")
        return 1

    print(f"Asset validation passed: {len(data['assets'])} runtime assets.")
    return 0

if __name__ == "__main__":
    sys.exit(main())
