#ifndef CYBERCBA_COMBAT_COMBATSTATE_H
#define CYBERCBA_COMBAT_COMBATSTATE_H

#include <cstdint>

namespace cybercba::combat
{

// Result of a finished combat encounter (VS-001 §7.4).
enum class CombatResult : std::uint8_t
{
    Ongoing,
    Victory,
    Retreat,
};

// Deterministic, renderer-free combat state (VS-001 §7.1, TAD-001 §68).
// A "side" tracks a single aggregate resistance value rather than per-unit
// HP — deliberate simplification to keep combat inside GPD's 2-5 minute
// target (VS-001 §7.1).
//
// POD data holder: fields intentionally have no m_ prefix (ADR-001
// exception).
struct CombatState
{
    int playerResistance;
    int enemyResistance;
    bool playerCovered{};
    bool playerExposed{};
    bool alarmTriggered{};
    int round{1};

    CombatResult result() const
    {
        if (enemyResistance <= 0)
        {
            return CombatResult::Victory;
        }
        if (playerResistance <= 0)
        {
            return CombatResult::Retreat;
        }
        return CombatResult::Ongoing;
    }
};

} // namespace cybercba::combat

#endif // CYBERCBA_COMBAT_COMBATSTATE_H
