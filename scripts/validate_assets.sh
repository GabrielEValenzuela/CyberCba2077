#!/usr/bin/env bash
# Validates the generated runtime catalog. Requires ImageMagick 7 (`magick`).
set -euo pipefail

project_root="$(cd "$(dirname "$0")/.." && pwd)"
cd "$project_root"
command -v magick >/dev/null || { echo "ImageMagick 7 (magick) is required" >&2; exit 1; }

declare -a entries=(
  'assets/processed/characters/emma_idle.png:256x384'
  'assets/processed/characters/emma_idle_selection.png:256x384'
  'assets/processed/characters/emma_hacking.png:384x256'
  'assets/processed/characters/magga_idle.png:256x384'
  'assets/processed/characters/magga_impact.png:384x256'
  'assets/processed/props/shelter_bed.png:256x256'
  'assets/processed/props/shelter_desk.png:256x256'
  'assets/processed/props/industrial_door.png:256x384'
  'assets/processed/props/shelter_shelf.png:256x256'
  'assets/processed/props/neo_cordoba_map_panel.png:256x256'
  'assets/processed/props/emma_magga_photo.png:192x192'
  'assets/processed/props/rain_window.png:256x256'
  'assets/processed/props/luciernaga_transmitter.png:256x192'
  'assets/processed/buildings/shelter_exterior.png:512x512'
  'assets/processed/buildings/neometro_entrance.png:512x512'
  'assets/processed/props/security_drone.png:256x256'
  'assets/processed/environment/surveillance_lamp.png:256x384'
  'assets/processed/props/street_barricade.png:384x256'
  'assets/processed/props/access_terminal.png:256x384'
  'assets/processed/evidence/insignia.png:128x96'
  'assets/processed/evidence/unsent_message.png:128x96'
  'assets/processed/evidence/train41_record.png:128x96'
)

for entry in "${entries[@]}"; do
  path="${entry%%:*}"
  expected="${entry#*:}"
  actual="$(magick identify -format '%wx%h %[channels]' "$path")"
  [[ "$actual" == "$expected srgba 4.0" ]] || { echo "invalid canvas/alpha: $path ($actual)" >&2; exit 1; }
  corner_alpha="$(magick "$path" -crop 1x1+0+0 -alpha extract -format '%[mean]' info:)"
  [[ "$corner_alpha" == "0" ]] || { echo "opaque top-left corner: $path" >&2; exit 1; }
done

jq -e '.schemaVersion == 7 and ([.assets[] | select(.processed != null)] | length == 24)' assets/data/assets-manifest.json >/dev/null
while IFS= read -r processed; do
  [[ -f "$processed" ]] || { echo "manifest path is missing: $processed" >&2; exit 1; }
done < <(jq -r '.assets[] | select(.processed != null) | .processed' assets/data/assets-manifest.json)
while IFS= read -r source; do
  [[ -f "$source" ]] || { echo "manifest source is missing: $source" >&2; exit 1; }
done < <(jq -r '.assets[] | select(.source != null) | .source' assets/data/assets-manifest.json)

echo "Asset validation passed: 22 runtime PNGs with point-ready alpha canvases."
