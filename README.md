# CyberCBA 2077

A narrative sci-fi RPG set in NeoCBA, a fictional cyberpunk Córdoba. Built with C++17, raylib, and CMake, using student-implemented data structures instead of the STL.

This is an academic project: the game itself isn't educational fiction, but building it is the assignment — students implement the custom ADTs, ownership model, and gameplay systems described in the docs below.

## Documentation

- [`docs/GPD.md`](docs/GPD.md) — Game Production & Design Document: what the game is.
- [`docs/TAD-001.md`](docs/TAD-001.md) — Technical Architecture Document: how it's built.
- [`docs/specs/VS-001.md`](docs/specs/VS-001.md) — Vertical Slice spec: exact scenes, puzzles, combat.
- [`docs/adr/`](docs/adr/) — Architecture Decision Records (style conventions, dependency management).
- [`CHANGELOG.md`](CHANGELOG.md) — release history.
- [`AGENTS.md`](AGENTS.md) / [`CLAUDE.md`](CLAUDE.md) — instructions for coding agents working in this repo.

## Building

Requires [vcpkg](https://github.com/microsoft/vcpkg) and CMake 3.21+.

```bash
export VCPKG_ROOT=/path/to/vcpkg
cmake --preset dev
cmake --build build/dev
ctest --test-dir build/dev --output-on-failure
```

Other presets (`asan`, `ci`, `release`, `debug`) are in `CMakePresets.json`.

Run the game with `./build/dev/src/app/cybercba_app` from the repo root (it looks for the `credits/` folder relative to the working directory).

## Contributing your name to the credits

The in-game credits screen reads every file in [`credits/`](credits/) automatically — no code changes needed. See [`credits/README.md`](credits/README.md) for the one-file-per-person workflow.

## License

See [`LICENSE`](LICENSE).
