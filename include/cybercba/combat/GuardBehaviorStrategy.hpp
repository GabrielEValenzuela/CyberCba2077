#ifndef CYBERCBA_COMBAT_GUARDBEHAVIORSTRATEGY_H
#define CYBERCBA_COMBAT_GUARDBEHAVIORSTRATEGY_H

#include "cybercba/combat/CombatState.hpp"

namespace cybercba::combat
{

// Strategy pattern for guard retaliation (VS-001 §9.5.2). CombatSystem picks
// one concrete strategy per encounter based on CombatState::alarmTriggered
// and calls it once per round instead of branching on that flag inline.
//
// Ownership: strategies are stateless behavior objects. CombatSystem holds
// them as observer pointers to instances it does not own (see
// GuardStrategies below).
class IGuardBehaviorStrategy
{
  public:
    virtual ~IGuardBehaviorStrategy() = default;

    virtual int decideDamage(const CombatState& state) const = 0;
};

// Regular guard: fixed base damage per round (VS-001 §7.3).
class StandardGuardStrategy final : public IGuardBehaviorStrategy
{
  public:
    int decideDamage(const CombatState& state) const override;
};

// Guard already alerted by a failed stealth/puzzle check (alarmTriggered):
// higher damage per round (VS-001 §7.3, §3.4).
class AlertedGuardStrategy final : public IGuardBehaviorStrategy
{
  public:
    int decideDamage(const CombatState& state) const override;
};

// Two stateless, reusable strategy instances owned by CombatSystem's
// translation unit (static storage duration) — no per-encounter allocation
// (VS-001 §9.5.3).
struct GuardStrategies
{
    static const IGuardBehaviorStrategy& standard();
    static const IGuardBehaviorStrategy& alerted();
};

} // namespace cybercba::combat

#endif // CYBERCBA_COMBAT_GUARDBEHAVIORSTRATEGY_H
