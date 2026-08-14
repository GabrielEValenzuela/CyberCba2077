#!/usr/bin/env bash
# Validates the generated runtime catalog. Requires ImageMagick (`magick` or legacy `identify`/`convert`).
set -euo pipefail

project_root="$(cd "$(dirname "$0")/.." && pwd)"
cd "$project_root"
if command -v magick >/dev/null; then
  im_identify_cmd=(magick identify)
  im_convert_cmd=(magick)
elif command -v identify >/dev/null && command -v convert >/dev/null; then
  im_identify_cmd=(identify)
  im_convert_cmd=(convert)
else
  echo "ImageMagick is required (magick or identify/convert)" >&2
  exit 1
fi

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
  actual_size="$("${im_identify_cmd[@]}" -format '%wx%h' "$path")"
  actual_channels="$("${im_identify_cmd[@]}" -format '%[channels]' "$path")"
  [[ "$actual_size" == "$expected" ]] || { echo "invalid canvas: $path ($actual_size, expected $expected)" >&2; exit 1; }
  [[ "${actual_channels,,}" == *rgba* ]] || { echo "invalid alpha channels: $path ($actual_channels)" >&2; exit 1; }
  corner_alpha="$("${im_convert_cmd[@]}" "$path" -crop 1x1+0+0 -alpha extract -format '%[fx:mean]' info:)"
  awk -v alpha="$corner_alpha" 'BEGIN { exit ((alpha + 0.0) <= 0.000001 ? 0 : 1) }' \
    || { echo "opaque top-left corner: $path (alpha=$corner_alpha)" >&2; exit 1; }
done

jq -e '.schemaVersion == 7 and ([.assets[] | select(.processed != null)] | length == 24)' assets/data/assets-manifest.json >/dev/null
while IFS= read -r processed; do
  [[ -f "$processed" ]] || { echo "manifest path is missing: $processed" >&2; exit 1; }
done < <(jq -r '.assets[] | select(.processed != null) | .processed' assets/data/assets-manifest.json)
while IFS= read -r source; do
  [[ -f "$source" ]] || { echo "manifest source is missing: $source" >&2; exit 1; }
done < <(jq -r '.assets[] | select(.source != null) | .source' assets/data/assets-manifest.json)

echo "Asset validation passed: 22 runtime PNGs with point-ready alpha canvases."
