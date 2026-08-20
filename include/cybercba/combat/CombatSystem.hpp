#ifndef CYBERCBA_COMBAT_COMBATSYSTEM_H
#define CYBERCBA_COMBAT_COMBATSYSTEM_H

#include "cybercba/combat/CombatAction.hpp"
#include "cybercba/combat/CombatState.hpp"
#include "cybercba/combat/GuardBehaviorStrategy.hpp"
#include "cybercba/game/CampaignState.hpp"

namespace cybercba::combat
{

// Deterministic tactical combat resolution (GPD §16, VS-001 §7). No grid, no
// per-unit HP, no randomness — every round is a pure function of the current
// CombatState and the chosen action, so it is fully unit-testable without
// raylib (TAD-001 §68).
//
// Ownership: stateless service. Holds no data of its own; every call takes
// the CombatState to mutate by reference. The caller (MissionSystem) owns
// that CombatState's lifetime.
class CombatSystem final
{
  public:
    // Returns false (without mutating state) if `action` is not legal for
    // `character` right now — e.g. UseResource without an EMP charge,
    // Maneuver as Emma, or Retreat before round 2 (VS-001 §7.2, §7.4).
    bool isActionAvailable(const CombatState& state, CombatActionType action, game::PlayerCharacter character,
                           bool hasEmpCharge) const;

    // Applies the player's action, then the guard strategy's retaliation for
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
