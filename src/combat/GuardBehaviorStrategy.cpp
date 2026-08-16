#include "cybercba/combat/GuardBehaviorStrategy.hpp"

namespace cybercba::combat
{

namespace
{

constexpr int STANDARD_GUARD_DAMAGE = 8;
constexpr int ALERTED_GUARD_DAMAGE  = 12;

// Stateless singletons backing GuardStrategies (VS-001 §9.5.3): one instance
// per translation unit, never allocated per-encounter.
static const StandardGuardStrategy gs_standardGuardStrategy;
static const AlertedGuardStrategy gs_alertedGuardStrategy;

} // namespace

int StandardGuardStrategy::decideDamage(const CombatState& /*state*/) const
{
    return STANDARD_GUARD_DAMAGE;
}

int AlertedGuardStrategy::decideDamage(const CombatState& /*state*/) const
{
    return ALERTED_GUARD_DAMAGE;
}

const IGuardBehaviorStrategy& GuardStrategies::standard()
{
    return gs_standardGuardStrategy;
}

const IGuardBehaviorStrategy& GuardStrategies::alerted()
{
    return gs_alertedGuardStrategy;
}

} // namespace cybercba::combat
