#pragma once

#include "cybercba/Encounter.hpp"

namespace cybercba
{

    // Configuration (participants + actions) for the v0.1.0 prologue's
    // protagonist-specific turn-based encounters, built on top of the
    // Phase 1 generic Encounter engine (ADR 0010-generic-encounter-system.md).
    //
    // Emma: hacking encounter against signal interference (Analyze/Isolate/
    // Redirect/Hide/Extract/Abort vs Trace/Signal Stability/Link Energy).
    Encounter buildEmmaHackingEncounter();

    // Magga: turn-based combat against a security drone (Strike/Guard/Dodge/
    // Tactical Impact vs Momentum).
    Encounter buildMaggaCombatEncounter();

} // namespace cybercba
