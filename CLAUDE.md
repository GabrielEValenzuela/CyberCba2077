# CLAUDE.md

See [`AGENTS.md`](AGENTS.md) — same instructions, kept in one file so they
don't drift out of sync between tools. Read that first.

Claude Code-specific notes:

- This repo uses TaskCreate/TaskUpdate for multi-step work in a session —
  prefer that over ad-hoc todo comments when a task has 3+ steps.
- When a change touches `docs/GPD.md`, `docs/TAD-001.md`, or
  `docs/specs/VS-001.md`, treat that as a deliberate, reviewable decision —
  confirm with the user before editing canonical narrative/architecture
  content, per TAD-001 §64 ("Claude no debe reescribir arquitectura completa
  salvo requerimiento explícito").
