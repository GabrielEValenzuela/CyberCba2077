# v0.1.0 static-art pilot

This directory contains six opaque crops derived from the generated character sheets for identity and expression review only. They are deliberately not runtime portraits: the final contract requires individual 1024px transparent PNGs.

The two scene-background pilots live in `assets/generated/backgrounds/`. Their 16:9 composition and safe-area layout are useful for review, but their 1672×941 source output is below the 1920×1080 final-master minimum. Do not wire any of these pilot files into `GameApp` or the main asset manifest.

Review the exact asset inventory in `assets/manifests/prologue-pilot.json` and the saved prompts in `assets/prompts/prologue/`.
