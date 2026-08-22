# Changelog

All notable changes to CyberCBA 2077 are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project uses `vMAJOR.MINOR.PATCH` versioning per TAD-001 §45
(`v0.x.y` during initial development; `v0.1.0` marks the Vertical Slice
architecture pass).

## [0.1.0] — 2026-08-16

### Added — Documentation

- `docs/specs/VS-001.md`: full Vertical Slice specification (Prologue + first
  Act I mission), scene-by-scene, per GPD-001 §77 / TAD-001 §67.
- `docs/adr/ADR-001-cpp-style-guide-merge.md`: merges an external C++ style
  guide into TAD-001's conventions (`m_` member prefix, `#ifndef` header
  guards, Allman braces, 120-column limit) while keeping TAD-001/GPD-001's
  ban on STL containers and standard smart pointers fully in force.
- `docs/adr/ADR-002-vcpkg-dependency-management.md`: switches dependency
  management from CMake `FetchContent` to vcpkg manifest mode.

### Added — Architecture & core gameplay systems (`cybercba_core`)

- `GameStateMachine` / `GameStateId`: explicit, table-driven transitions
  between `MainMenu → CharacterSelection → Prologue → CentralMap → Mission →
  {Puzzle, Combat} → PostMission`, plus pause/resume.
- `Game`: composition root for campaign state; drives the state machine from
  an `InputSnapshot` and loads the credits screen text once at construction.
- `CampaignState`: player choice (Emma/Magga), prologue narrative flags
  (`alarmTriggered`, `intelComplete`, `missionOutcome`), and VS-001 resources
  (`empCharges`, `coverBonus`).
- `CombatSystem` + `IGuardBehaviorStrategy` (Strategy pattern:
  `StandardGuardStrategy`/`AlertedGuardStrategy`): deterministic tactical
  combat resolution (attack/cover/retreat/maneuver/use-resource), no grid, no
  RNG — fully unit-testable without raylib.
- `DialogueSystem` / `DialogueScript` / `DialogueChoice`: line-by-line
  dialogue traversal with branching decisions.
- `IPersuasionStrategy` (Strategy pattern: `HackerProofStrategy`/
  `IntimidationStrategy`): NPC cooperation resolved per protagonist instead
  of branching inline.
- `MissionSystem`: orchestrates a mission's Dialogue/Puzzle/Combat scene
  sequence; deliberately does not own concrete Puzzle/Combat instances, so it
  stays fully link-safe and testable independent of the custom structures.
- `ProgressionSystem` / `SkillProgress` / `SkillDefinition`: the 15-skill
  catalog (5 common + 5 per protagonist) and Level I → II upgrade logic.
- `CreditsLoader`: reads every contributor file under `credits/` (skipping
  `README.md` and `_`-prefixed templates), sorted and trimmed — ported from
  the project's previous iteration, ownership/ADT rewritten to this
  codebase's raw-pointer/no-STL-container conventions.
- `IPuzzle` + `EnergyRedirectionPuzzle` / `TransmissionDecodingPuzzle` /
  `SurveillanceInterferencePuzzle`: VS-001's three puzzle families, unified
  under a common interface.
- `Logger`, `InputSnapshot`: minimal logging levels and the
  platform-independent input representation.

### Added — Student exercise scope (intentional, not a gap)

- `DynamicArray<T>`, `Queue<T>`, `Stack<T>`, `DirectedGraph<T>`
  (`include/cybercba/structures/`): public API headers only, fully
  documented contracts and ownership rules, **no implementation bodies** —
  reserved as a data-structures exercise for the development team per
  GPD-001 §55. `PuzzleSystem`'s object file compiles against these headers
  but is isolated in its own CMake object library (`cybercba_core_puzzle`)
  so the rest of the tree builds and links without waiting on them.

### Added — Platform layer (`cybercba_raylib`) & app (`cybercba_app`)

- `InputAdapter`, `AssetStore` (placeholder solid-color textures pending real
  art), `AudioAdapter`, `Renderer` — raylib-only adapters; `cybercba_core`
  never includes `<raylib.h>`.
- `GameStateId::Credits`: an in-fiction-neutral credits screen reachable from
  the main menu (`cancel` toggles it), rendering `CreditsLoader`'s output.
- `src/app/main.cpp`: composition root and game loop; verified to open a
  real window, load placeholder textures, and run without crashing.

### Added — Build, test, and CI infrastructure

- CMake targets: `cybercba_core`, `cybercba_core_puzzle`, `cybercba_raylib`,
  `cybercba_app`, `cybercba_tests`, matching TAD-001 §7.
- `vcpkg.json` (manifest mode, pinned `builtin-baseline`): raylib 5.5#1,
  GoogleTest 1.17.0#2. See ADR-002 for the version rationale.
- `CMakePresets.json`: `dev`, `debug`, `asan`, `release`, `ci` presets, all
  wired to the vcpkg toolchain.
- 38 GoogleTest unit tests covering `GameStateMachine`, `CombatSystem`,
  `IGuardBehaviorStrategy`, `DialogueSystem`, `MissionSystem`,
  `IPersuasionStrategy`, `ProgressionSystem`/`SkillDefinition`, and
  `CreditsLoader` — all passing locally under both a plain Debug build and
  Clang + ASan/UBSan.
- `.github/workflows/ci.yml` + `.github/actions/setup-vcpkg`: `format`,
  `forbidden-symbols-audit` (greps for banned STL containers/smart
  pointers), `clang-build`, `tests`, `clang-tidy`, `sanitizers`, and a
  Windows/MSVC `portability` job, per TAD-001 §37-§39.
- `.clang-format` and `.clang-tidy` updated to the ADR-001 conventions.

### Removed

- The entire previous codebase (`src/`, `include/`, `tests/`, `benchmarks/`,
  old CI workflows, `CMakeLists.txt`, etc.) — this release is a from-scratch
  reimplementation against GPD-001/TAD-001/VS-001, per explicit project
  direction. `GPD.md`/`TAD.md` were preserved and mirrored into `docs/`.

### Known limitations of this release

- `PuzzleSystem`'s three families are implemented against the structures'
  public API but cannot link into a running executable until
  `DynamicArray`/`Queue`/`Stack`/`DirectedGraph` are implemented (intentional
  — see "Student exercise scope" above).
- `Game`/`MissionSystem` are not yet wired to the actual Prologue/Mission 1
  narrative content from VS-001 (dialogue text, scene sequencing) — only the
  architectural skeleton (`MainMenu → CharacterSelection → Prologue →
  CentralMap`) runs today. Authoring that content is the next milestone.
- Assets are procedurally generated solid-color placeholders; no real art
  exists yet (VS-001 §13 lists what's needed).
