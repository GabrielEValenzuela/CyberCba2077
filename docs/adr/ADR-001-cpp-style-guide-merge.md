# ADR-001: Merge external C++ style guide with TAD-001 conventions

## Status

Accepted

## Context

TAD-001 §22-§25 defined naming and formatting conventions (camelCase_ members,
#pragma once, LLVM-based clang-format). The user supplied a separate, more
detailed C++ style guide (Sonarr/OSQuery-style: m_ prefix, ifndef guards,
PascalCase enum values, const/noexcept/final discipline, RAII rules, etc.)
and asked that it govern the project going forward.

Some of that guide's rules directly contradict GPD-001's core academic
constraint: no STL containers, no `std::unique_ptr`/`std::shared_ptr`
(GPD §2/§51, TAD §10/§11). That constraint is the pedagogical point of the
project (students implement their own ADTs and ownership) and is not
negotiable via a general style guide.

## Decision

1. **GPD's STL/smart-pointer ban is authoritative and unaffected.** Raw
   pointers remain the normal non-owning idiom; owning pointers use custom
   wrappers, not `std::unique_ptr`/`std::shared_ptr`. `std::vector`/`std::set`/
   etc. remain forbidden. Any rule in the new guide that assumes their use
   (e.g. "heap objects must be managed by smart pointers", "prefer
   std::vector") does not apply to this project.
2. **All other naming/style rules from the new guide supersede TAD-001
   §22-§25.** Specifically adopted:
   - Member variables: `m_` prefix (not `camelCase_`).
   - Static member variables: `ms_` prefix.
   - Namespace/global variables: `g_`; static-at-namespace-scope: `gs_`;
     function-local static: `s_`.
   - Pointer variables: `p` prefix (e.g. `pOwner`) where a raw pointer is used
     as an observer per TAD-001 §12.
   - Header guards: `#ifndef HEADER_FILENAME_H` / `#define` / `#endif`,
     replacing `#pragma once`. **Deviation from the literal example given:**
     no leading underscore — `_Name` / `_UPPERCASE` identifiers are reserved
     for the implementation in C++ (clang-tidy `bugprone-reserved-identifier`,
     which stays enabled as `WarningsAsErrors`), so guards are named
     `CYBERCBA_<PATH>_H` instead of `_CYBERCBA_<PATH>_H`.
   - Enum values: PascalCase (already TAD-001 policy, unchanged).
   - Class/function/variable case rules: unchanged from TAD-001 (PascalCase
     classes, camelCase functions/variables) — the new guide agrees with
     TAD-001 here.
   - Braces: new line before opening brace (Allman), except an empty function
     body which may stay on one line.
   - Always brace single-statement `if`/`for`/`while` bodies.
   - Brace-initialization (`int result{ EXIT_SUCCESS };`) preferred over `=`.
   - `const`-correctness and `noexcept` discipline as described in the new
     guide.
   - `final` on classes not designed for further derivation.
   - `using` instead of `typedef`.
   - No magic numbers — named `constexpr` constants.
   - Avoid `get` prefix on accessors; avoid repeating the class name in
     methods that act on `this` as a whole.
   - 120-column line limit (raised from TAD-001's 100).
3. `.clang-format` and `.clang-tidy` are updated to encode the merged rules
   where automatable (brace style, column limit, naming where clang-tidy's
   `readability-identifier-naming` supports it directly).
4. Exceptions vs. return codes: adopt the new guide's rule (exceptions for
   non-ignorable failures) for `cybercba_core`/`cybercba_raylib`/`cybercba_app`
   code written from this point forward. This does not retroactively change
   already-agreed VS-001 semantics (e.g. `bool` returns on `transitionTo`,
   `applyUpgrade`, etc. represent *ignorable* precondition failures per their
   own contracts, which the new guide explicitly allows as a documented
   exception).

## Alternatives considered

- Keep TAD-001 conventions unchanged, ignore the new guide: rejected per
  explicit user instruction.
- Adopt the new guide fully, including STL containers/smart pointers: rejected
  — would invalidate GPD's stated academic constraint without an explicit,
  unambiguous instruction to change the project's grading premise. The user
  was asked directly and chose to keep the ban.

## Consequences

- TAD-001 §22-§25 are superseded by this ADR for everything except the
  STL/smart-pointer prohibition, which remains sourced from TAD-001 §10-§11
  and GPD §51-§52.
- All code written prior to this ADR (Logger, GameStateMachine, CampaignState,
  SkillDefinition, ProgressionSystem, DialogueSystem, and their structure
  headers) must be updated to match: `#pragma once` → include guards, member
  prefix `camelCase_` → `m_camelCase`, brace style Attach → Allman.
- Per TAD-001 §2, this ADR now sits above old TAD-001 sections it amends but
  below GPD-001 and VS-001 in the hierarchy.
