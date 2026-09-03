#include "cybercba/combat/GuardBehaviorStrategy.hpp"

namespace cybercba::combat
{

namespace
{

constexpr int STANDARD_GUARD_DAMAGE = 8;
constexpr int ALERTED_GUARD_DAMAGE  = 12;

// EscalatingGuardStrategy: base damage plus a per-level increment. Chosen so
// that even at nivelAlerta == 1 (the weakest propagated alert) the damage
// already exceeds AlertedGuardStrategy's fixed value, matching the idea that
// this strategy tracks *how* alerted the guard is, not just whether.
constexpr int ESCALATING_BASE_DAMAGE      = 8;
constexpr int ESCALATING_DAMAGE_PER_LEVEL = 5;

// Stateless singletons backing GuardStrategies (VS-001 §9.5.3): one instance
// per translation unit, never allocated per-encounter.
//
// clang-tidy's GlobalConstant category covers any file-scope const
// regardless of the `static` keyword, conflating numeric constexpr
// constants (UPPER_CASE, see ADR-001) with these object singletons (gs_
// prefix, matching ADR-001's "static at global/namespace scope" rule).
// ADR-001 intends the two differently; the NOLINTNEXTLINE markers below are
// a deliberate, understood suppression, not a blanket NOLINT.
// NOLINTNEXTLINE(readability-identifier-naming)
static const StandardGuardStrategy gs_standardGuardStrategy;
// NOLINTNEXTLINE(readability-identifier-naming)
static const AlertedGuardStrategy gs_alertedGuardStrategy;
// NOLINTNEXTLINE(readability-identifier-naming)
static const EscalatingGuardStrategy gs_escalatingGuardStrategy;

} // namespace

int StandardGuardStrategy::decideDamage(const CombatState& /*state*/, int /*nivelAlerta*/) const
{
    return STANDARD_GUARD_DAMAGE;
}

int AlertedGuardStrategy::decideDamage(const CombatState& /*state*/, int /*nivelAlerta*/) const
{
    return ALERTED_GUARD_DAMAGE;
}

int EscalatingGuardStrategy::decideDamage(const CombatState& /*state*/, int nivelAlerta) const
{
    return ESCALATING_BASE_DAMAGE + nivelAlerta * ESCALATING_DAMAGE_PER_LEVEL;
}

const IGuardBehaviorStrategy& GuardStrategies::standard()
{
    return gs_standardGuardStrategy;
}

const IGuardBehaviorStrategy& GuardStrategies::alerted()
{
    return gs_alertedGuardStrategy;
}

const IGuardBehaviorStrategy& GuardStrategies::escalating()
{
    return gs_escalatingGuardStrategy;
}

} // namespace cybercba::combat
