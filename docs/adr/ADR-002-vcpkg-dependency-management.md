# ADR-002: Use vcpkg (manifest mode) for all dependencies

## Status

Accepted

## Context

The project initially fetched raylib and GoogleTest via CMake `FetchContent`,
building both from source on every fresh configure. The user asked that the
whole project use vcpkg instead. TAD-001 §35 requires dependencies to be
pinned to an exact release/tag, never `master`/`latest`.

vcpkg's registry (pinned via `builtin-baseline`, commit
`d034cf165b78579b6d526cf440d2781940e9074b`) currently offers:

- `raylib` at version `5.5#1` (not `6.0` — 6.0 is not yet packaged in vcpkg's
  registry at the pinned baseline).
- `gtest` at version `1.17.0#2` (not `1.18.0`, the version TAD-001 §35 and the
  earlier FetchContent setup used).

## Decision

1. Add `vcpkg.json` (manifest mode) pinning `raylib >= 5.5#1` and
   `gtest >= 1.17.0#2`, with `builtin-baseline` fixed to the commit above —
   satisfies TAD-001 §35's "exact tag/commit, never latest" rule via the
   registry-version mechanism vcpkg provides instead of a raw git tag.
2. `CMakePresets.json` gains a hidden `vcpkg-base` preset setting
   `toolchainFile` to `$env{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake`;
   every configure preset inherits it. `VCPKG_ROOT` must be set in the
   environment (locally or in CI) — this mirrors the original repo's own CI
   convention (`VCPKG_ROOT: ${{ github.workspace }}/vcpkg`).
3. `src/platform/raylib/CMakeLists.txt` and `tests/CMakeLists.txt` now use
   `find_package(raylib CONFIG REQUIRED)` / `find_package(GTest CONFIG
   REQUIRED)` instead of `FetchContent_Declare`/`FetchContent_MakeAvailable`.
4. **Known port gap, worked around locally:** vcpkg's `raylib` port ships a
   legacy pkg-config-style `raylib-config.cmake` whose `raylib` target does
   not propagate its own link dependencies (`glfw3`, X11) on Linux static
   builds. `src/platform/raylib/CMakeLists.txt` explicitly
   `find_package(glfw3 CONFIG REQUIRED)` and `find_package(X11 REQUIRED)` and
   links them alongside `raylib` on Unix-non-Apple. This is a local,
   reversible workaround confined to that one CMakeLists.txt — if a future
   vcpkg raylib port version fixes the propagation, the extra
   `find_package`/link calls can be removed without touching anything else.
5. CI (`.github/workflows/ci.yml`) clones vcpkg pinned to the same commit as
   `builtin-baseline`, bootstraps it, and caches the checkout plus
   `vcpkg_installed/` keyed on `vcpkg.json`'s hash, via a shared composite
   action (`.github/actions/setup-vcpkg`) reused by every job that builds.
6. TAD-001 §35's literal version numbers (raylib 6.0, GoogleTest 1.18.0) are
   superseded by this ADR for the versions actually pinned in `vcpkg.json`.
   If vcpkg's registry later carries raylib 6.0, upgrading is a manifest-only
   change (bump the version constraint and `builtin-baseline`), not an
   architecture change.

## Alternatives considered

- Keep `FetchContent`: rejected per explicit user instruction to standardize
  on vcpkg for the whole project.
- Use vcpkg in classic (non-manifest) mode: rejected — manifest mode
  (`vcpkg.json` + `builtin-baseline`) is what makes dependency versions
  reproducible and reviewable in a PR diff, which is the same reproducibility
  goal TAD-001 §35 is written for.
- Pin an overlay port for raylib 6.0: rejected as unnecessary complexity for
  this stage; 5.5 satisfies everything VS-001 currently needs (window,
  input, 2D texture drawing, audio device lifecycle). Revisit via a future
  ADR if a 6.0-specific API becomes required.

## Consequences

- Contributors need `VCPKG_ROOT` set (or a local vcpkg checkout) to configure
  the project; this is documented in the top-level README/CLAUDE.md build
  instructions.
- First configure on a clean machine builds raylib's dependency chain
  (glfw3, stb, qoi, cgltf, miniaudio, etc.) from source via vcpkg, same cost
  class as the previous FetchContent approach, but now cached by vcpkg's
  binary caching and pinned to reviewable exact versions.
- Linux builds require `libx11-dev`/`libwayland-dev`/etc. system packages
  present for glfw3's own from-source build — unchanged from before, just
  now installed for vcpkg's build instead of FetchContent's.
