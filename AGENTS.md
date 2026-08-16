# AGENTS.md

Instructions for any coding agent (Claude Code, Codex, Cursor, etc.) working
in this repository. Kept short on purpose — full context lives in the docs
below; read only what the current task needs instead of the whole tree.

## Read in this order, only as deep as the task requires

1. `docs/GPD.md` — product/narrative/gameplay truth. Read when the task
   touches *what the game does*.
2. `docs/TAD-001.md` — architecture/engineering rules. Read when the task
   touches *how code is structured*. Sections 22-25 (naming/format) are
   superseded — see ADR-001 below instead.
3. `docs/adr/ADR-001-cpp-style-guide-merge.md` — the actual naming/format
   rules in force (`m_` members, `#ifndef` guards, Allman braces, 120 cols).
4. `docs/adr/ADR-002-vcpkg-dependency-management.md` — dependency versions
   and vcpkg setup (supersedes TAD-001 §35's version numbers).
5. `docs/specs/VS-001.md` — the Vertical Slice spec (Prologue + Mission 1):
   exact scenes, puzzle/combat numbers, required classes. Read when
   implementing or changing gameplay content/logic.
6. Existing code + tests — the actual current state. Trust this over any
   doc summary (including this file) if they disagree; then fix the doc.

**Don't re-read a doc you've already loaded this session just to double-check
phrasing.** These docs don't change mid-task.

## Hard constraints (violating these is not a style choice, it fails CI)

- No STL containers (`std::vector`, `std::map`, `std::queue`, `std::stack`,
  `std::set`, etc.) and no `std::unique_ptr`/`std::shared_ptr` anywhere in
  `include/`/`src/`. Enforced by CI's `forbidden-symbols-audit` job (a grep,
  not a suggestion). `std::string`/`std::string_view` are fine.
- `cybercba_core` MUST NOT `#include <raylib.h>` or call any raylib
  function. Gameplay logic must be testable without a window.
- The four ADTs in `include/cybercba/structures/` (`DynamicArray`, `Queue`,
  `Stack`, `DirectedGraph`) are headers-only by design — a student exercise,
  not an oversight. Do not implement their bodies unless explicitly asked to.
  `PuzzleSystem` compiles against them but is isolated in the
  `cybercba_core_puzzle` object-library target precisely so the rest of the
  tree keeps linking while they're unimplemented.
- `.clang-format`/`.clang-tidy` are the actual style contract — run
  `clang-format -i` on touched files before considering a change done, don't
  hand-format.

## Build / test

```bash
export VCPKG_ROOT=~/vcpkg   # or wherever vcpkg is checked out
cmake --preset dev
cmake --build build/dev
ctest --test-dir build/dev --output-on-failure
```

Other presets: `asan` (Clang + ASan/UBSan), `ci` (Clang, warnings-as-errors,
clang-tidy), `release`. See `CMakePresets.json`.

## Current implementation state (2026-08, don't assume more than this)

Done and tested (38 unit tests, green under both `dev` and `asan`):
`GameStateMachine`, `Game`, `CampaignState`, `CombatSystem` +
`IGuardBehaviorStrategy`, `DialogueSystem`, `IPersuasionStrategy`,
`MissionSystem`, `ProgressionSystem`/`SkillDefinition`, `CreditsLoader`, the
raylib platform layer (`InputAdapter`/`AssetStore`/`AudioAdapter`/
`Renderer`), and `cybercba_app`'s composition root — it runs a real window.

Not done yet: the actual Prologue/Mission 1 narrative content (dialogue
text, scene data) from VS-001 isn't authored — `Game` only runs the
architectural skeleton (`MainMenu → CharacterSelection → Prologue →
CentralMap`). Real art assets don't exist (placeholders are procedurally
generated solid colors). The four custom structures and anything depending
on them (`PuzzleSystem`) are unimplemented by design (see above).

## When something is genuinely ambiguous

Per TAD-001 §71: don't invent canon or architecture. Either find the answer
in the doc order above, or implement the minimal reversible solution and
say so — don't silently assume.
