#!/usr/bin/env bash
# Produces runtime PNGs from the generated source set without overwriting it.
# Requires ImageMagick (`magick` or legacy `identify`/`convert`). Every resize uses point sampling.
set -euo pipefail

project_root="$(cd "$(dirname "$0")/.." && pwd)"
cd "$project_root"

if command -v magick >/dev/null; then
  im_convert_cmd=(magick)
elif command -v convert >/dev/null && command -v identify >/dev/null; then
  im_convert_cmd=(convert)
else
  echo "ImageMagick is required (magick or identify/convert)" >&2
  exit 1
fi

mkdir -p assets/raw/{characters,props,buildings,environment} \
  assets/processed/{characters,props,buildings,environment}

# process_asset source raw-path processed-path canvas-width canvas-height alpha-threshold
# The threshold is used only to locate a meaningful visual bound. The source alpha
# itself is retained in the crop, so crisp edges and intentional glows are preserved.
process_asset() {
  local source="$1" raw="$2" output="$3" width="$4" height="$5" threshold="$6"
  local geometry inner_width=$((width - 32)) inner_height=$((height - 32))

  install -m 0644 "$source" "$raw"
  geometry="$("${im_convert_cmd[@]}" "$source" -alpha extract -threshold "$threshold" -trim -format '%wx%h%O' info:)"
  "${im_convert_cmd[@]}" "$source" -alpha on -crop "$geometry" +repage \
    -filter point -resize "${inner_width}x${inner_height}" \
    -gravity center -background none -extent "${width}x${height}" \
    "$output"
}

# Interior: shelter
process_asset 'assets/data/ChatGPT Image Aug 2, 2026, 07_39_54 PM (2).png' \
  assets/raw/props/shelter_bed.png assets/processed/props/shelter_bed.png 256 256 3%
process_asset 'assets/data/ChatGPT Image Aug 2, 2026, 07_39_54 PM (3).png' \
  assets/raw/props/shelter_desk.png assets/processed/props/shelter_desk.png 256 256 3%
process_asset 'assets/data/ChatGPT Image Aug 2, 2026, 07_39_55 PM (4).png' \
  assets/raw/props/industrial_door.png assets/processed/props/industrial_door.png 256 384 3%
process_asset 'assets/data/ChatGPT Image Aug 2, 2026, 07_39_55 PM (5).png' \
  assets/raw/props/shelter_shelf.png assets/processed/props/shelter_shelf.png 256 256 3%
process_asset 'assets/data/ChatGPT Image Aug 2, 2026, 07_40_15 PM (3).png' \
  assets/raw/props/neo_cordoba_map_panel.png assets/processed/props/neo_cordoba_map_panel.png 256 256 3%
process_asset 'assets/data/ChatGPT Image Aug 2, 2026, 07_40_15 PM (4).png' \
  assets/raw/props/emma_magga_photo.png assets/processed/props/emma_magga_photo.png 192 192 3%
process_asset 'assets/data/ChatGPT Image Aug 2, 2026, 07_40_16 PM (5).png' \
  assets/raw/props/rain_window.png assets/processed/props/rain_window.png 256 256 3%
process_asset 'assets/data/ChatGPT Image Aug 2, 2026, 07_40_16 PM (6).png' \
  assets/raw/props/luciernaga_transmitter.png assets/processed/props/luciernaga_transmitter.png 256 192 3%

# Characters. The first Emma PNG in assets/data is retained only as raw archival
# source because its opaque black background connects to black clothing. The clean
# alpha variant below is the selected runtime source for the same idle role.
install -m 0644 'assets/data/ChatGPT Image Aug 2, 2026, 07_12_58 PM.png' assets/raw/characters/emma_idle_opaque_source.png
process_asset 'assets/data/ChatGPT Image Aug 2, 2026, 07_45_49 PM (1).png' \
  assets/raw/characters/emma_idle_alpha_source.png assets/processed/characters/emma_idle.png 256 384 3%
process_asset 'assets/data/ChatGPT Image Aug 2, 2026, 07_45_50 PM (2).png' \
  assets/raw/characters/emma_hacking.png assets/processed/characters/emma_hacking.png 384 256 3%
process_asset 'assets/data/ChatGPT Image Aug 2, 2026, 07_40_15 PM (1).png' \
  assets/raw/characters/magga_idle.png assets/processed/characters/magga_idle.png 256 384 3%
process_asset 'assets/data/ChatGPT Image Aug 2, 2026, 07_40_15 PM (2).png' \
  assets/raw/characters/magga_impact.png assets/processed/characters/magga_impact.png 384 256 3%

# Exterior: first level
process_asset 'assets/data/ChatGPT Image Aug 2, 2026, 07_45_50 PM (3).png' \
  assets/raw/buildings/shelter_exterior.png assets/processed/buildings/shelter_exterior.png 512 512 3%
process_asset 'assets/data/ChatGPT Image Aug 2, 2026, 07_45_50 PM (4).png' \
  assets/raw/buildings/neometro_entrance.png assets/processed/buildings/neometro_entrance.png 512 512 3%
process_asset 'assets/data/ChatGPT Image Aug 2, 2026, 07_45_50 PM (5).png' \
  assets/raw/props/security_drone.png assets/processed/props/security_drone.png 256 256 3%
process_asset 'assets/data/ChatGPT Image Aug 2, 2026, 07_45_50 PM (6).png' \
  assets/raw/environment/surveillance_lamp.png assets/processed/environment/surveillance_lamp.png 256 384 3%
process_asset 'assets/data/ChatGPT Image Aug 2, 2026, 07_45_51 PM (7).png' \
  assets/raw/props/street_barricade.png assets/processed/props/street_barricade.png 384 256 3%
process_asset 'assets/data/ChatGPT Image Aug 2, 2026, 07_45_51 PM (8).png' \
  assets/raw/props/access_terminal.png assets/processed/props/access_terminal.png 256 384 3%

# Selection-screen-only optical correction (see docs/character-design.md and
# assets-manifest.json id "character.emma.idle_selection"). Regenerated from the
# same raw source as emma_idle; never touches the World/gameplay sprite.
if command -v python3 >/dev/null; then
  python3 "$project_root/scripts/normalize_characters.py" >/dev/null
else
  echo "python3 not found: skipping character.emma.idle_selection regeneration" >&2
fi

echo "Processed runtime assets are available under assets/processed/."
