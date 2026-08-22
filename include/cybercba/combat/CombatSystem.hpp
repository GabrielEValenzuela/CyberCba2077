#ifndef CYBERCBA_COMBAT_COMBATSYSTEM_H
#define CYBERCBA_COMBAT_COMBATSYSTEM_H

#include "cybercba/combat/CombatAction.hpp"
#include "cybercba/combat/CombatState.hpp"
#include "cybercba/combat/GuardBehaviorStrategy.hpp"
#include "cybercba/game/CampaignState.hpp"

namespace cybercba::combat
{

/// Resolves deterministic tactical combat without a grid or randomness.
///
/// Every round is a function of the supplied state and action, making the
/// system unit-testable without raylib. This stateless service does not own
/// the CombatState it receives.
class CombatSystem final
{
  public:
    /// Returns false without mutating state if `action` is not legal for
    // `character` right now — e.g. UseResource without an EMP charge,
    // Maneuver as Emma, or Retreat before round 2 (VS-001 §7.2, §7.4).
    bool isActionAvailable(const CombatState& state, CombatActionType action, game::PlayerCharacter character,
                           bool hasEmpCharge) const;

    /// Applies the player's action, then the guard strategy's retaliation for
    // this round, and advances state.round. Returns false without mutating
    // state if the action is currently unavailable (see isActionAvailable).
    // Consuming CampaignState::empCharges for UseResource is the caller's
    // responsibility (CombatSystem does not depend on CampaignState beyond
    // reading PlayerCharacter for action legality).
    bool resolveAction(CombatState& state, CombatActionType action, game::PlayerCharacter character, bool hasEmpCharge,
                       const IGuardBehaviorStrategy& guardStrategy) const;
};

} // namespace cybercba::combat

#endif // CYBERCBA_COMBAT_COMBATSYSTEM_H
