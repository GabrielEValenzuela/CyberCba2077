#!/usr/bin/env python3
from __future__ import annotations
import hashlib, json, sys
from pathlib import Path
from PIL import Image

ROOT = Path(__file__).resolve().parents[1]
MANIFEST = ROOT / "assets/manifests/v0.1.0-prologue-batch-02-runtime.json"

def sha256_file(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(1024*1024), b""):
            h.update(chunk)
    return h.hexdigest()

def main() -> int:
    data = json.loads(MANIFEST.read_text(encoding="utf-8"))
    errors = []
    ids = set()
    for asset in data["assets"]:
        aid = asset["id"]
        if aid in ids:
            errors.append(f"duplicate id: {aid}")
        ids.add(aid)
        path = ROOT / asset["path"]
        if not path.exists():
            errors.append(f"missing: {asset['path']}")
            continue
        with Image.open(path) as img:
            if img.size != (asset["width"], asset["height"]):
                errors.append(f"{aid}: dimensions mismatch")
            if asset["alphaRequired"]:
                if "A" not in img.mode:
                    errors.append(f"{aid}: alpha channel required")
                else:
                    lo, hi = img.getchannel("A").getextrema()
                    if lo == 255:
                        errors.append(f"{aid}: fully opaque alpha")
                    if hi == 0:
                        errors.append(f"{aid}: fully transparent")
        if sha256_file(path) != asset["sha256"]:
            errors.append(f"{aid}: hash mismatch")
        if not asset.get("runtimeEligible"):
            errors.append(f"{aid}: not runtime eligible")
        if aid.startswith("portrait_magga"):
            traits = " ".join(asset.get("canonicalTraits", [])).lower()
            if "blonde" not in traits or "green eyes" not in traits:
                errors.append(f"{aid}: canonical Magga traits missing")
            lock = asset.get("identityLock", {})
            if lock.get("hairColor") != "blonde":
                errors.append(f"{aid}: Magga hair lock is not blonde")
    if errors:
        print("Validation failed:")
        print("\n".join(f" - {e}" for e in errors))
        return 1
    print(f"Validation passed: {len(data['assets'])} runtime assets.")
    print("Magga identity lock: blonde hair + green eyes.")
    return 0

if __name__ == "__main__":
    sys.exit(main())
