#ifndef CYBERCBA_COMBAT_COMBATACTION_H
#define CYBERCBA_COMBAT_COMBATACTION_H

#include <cstdint>

namespace cybercba::combat
{

/// Player-chosen combat actions. Attack/Cover/Retreat
// are common to both protagonists; Maneuver is Magga-exclusive and
// UseResource is Emma-exclusive (consumes CampaignState::empCharges).
enum class CombatActionType : std::uint8_t
{
    Attack,
    Cover,
    Retreat,
    Maneuver,
    UseResource,
};

} // namespace cybercba::combat

#endif // CYBERCBA_COMBAT_COMBATACTION_H
