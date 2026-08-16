#include "cybercba/combat/GuardBehaviorStrategy.hpp"

namespace cybercba::combat
{

namespace
{

constexpr int STANDARD_GUARD_DAMAGE = 8;
constexpr int ALERTED_GUARD_DAMAGE  = 12;

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
    static const StandardGuardStrategy s_instance;
    return s_instance;
}

const IGuardBehaviorStrategy& GuardStrategies::alerted()
{
    static const AlertedGuardStrategy s_instance;
    return s_instance;
}

} // namespace cybercba::combat
