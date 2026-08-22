#ifndef CYBERCBA_COMBAT_COMBATSTATE_H
#define CYBERCBA_COMBAT_COMBATSTATE_H

#include <cstdint>

namespace cybercba::combat
{

/// Result of a finished combat encounter.
enum class CombatResult : std::uint8_t
{
    Ongoing,
    Victory,
    Retreat,
};

/// Deterministic, renderer-free combat state.
///
/// A side tracks a single aggregate resistance value rather than per-unit HP.
//
/// POD data holder; its fields are intentionally directly accessible.
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
