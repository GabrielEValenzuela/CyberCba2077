#include "cybercba/combat/CombatSystem.hpp"

namespace cybercba::combat
{

namespace
{

constexpr int ATTACK_DAMAGE                  = 10;
constexpr int MANEUVER_DAMAGE                = 16;
constexpr int USE_RESOURCE_DAMAGE            = 8;
constexpr int COVER_DAMAGE_REDUCTION_PERCENT = 50;
constexpr int MIN_ROUND_FOR_RETREAT          = 2;

int applyCoverReduction(int damage, bool covered)
{
    if (!covered)
    {
        return damage;
    }
    return damage - (damage * COVER_DAMAGE_REDUCTION_PERCENT / 100);
}

} // namespace

bool CombatSystem::isActionAvailable(const CombatState& state, CombatActionType action, game::PlayerCharacter character,
                                     bool hasEmpCharge) const
{
    switch (action)
    {
    case CombatActionType::Attack:
    case CombatActionType::Cover:
        return true;
    case CombatActionType::Retreat:
        return state.round >= MIN_ROUND_FOR_RETREAT;
    case CombatActionType::Maneuver:
        return character == game::PlayerCharacter::Magga;
    case CombatActionType::UseResource:
        return character == game::PlayerCharacter::Emma && hasEmpCharge;
    }
    return false;
}

bool CombatSystem::resolveAction(CombatState& state, CombatActionType action, game::PlayerCharacter character,
                                 bool hasEmpCharge, const IGuardBehaviorStrategy& guardStrategy) const
{
    if (!isActionAvailable(state, action, character, hasEmpCharge))
    {
        return false;
    }

    state.playerExposed = false;

    switch (action)
    {
    case CombatActionType::Attack:
        state.enemyResistance -= ATTACK_DAMAGE;
        break;
    case CombatActionType::Cover:
        state.playerCovered = true;
        break;
    case CombatActionType::Retreat:
        state.playerResistance = 0;
        return true;
    case CombatActionType::Maneuver:
        state.enemyResistance -= MANEUVER_DAMAGE;
        state.playerExposed = true;
        break;
    case CombatActionType::UseResource:
        state.enemyResistance -= USE_RESOURCE_DAMAGE;
        break;
    }

    if (state.result() != CombatResult::Ongoing)
    {
        return true;
    }

    const int guardDamage = guardStrategy.decideDamage(state);
    state.playerResistance -= applyCoverReduction(guardDamage, state.playerCovered);
    state.playerCovered = false;
    ++state.round;

    return true;
}

} // namespace cybercba::combat
