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

    // nivelAlerta: this guard's own alert level from GuardSquad::propagarAlerta
    // (Issue #220), independent of CombatState::alarmTriggered. Standard and
    // Alerted ignore it — only EscalatingGuardStrategy uses it.
    virtual int decideDamage(const CombatState& state, int nivelAlerta) const = 0;
};

// Regular guard: fixed base damage per round (VS-001 §7.3).
class StandardGuardStrategy final : public IGuardBehaviorStrategy
{
  public:
    int decideDamage(const CombatState& state, int nivelAlerta) const override;
};

// Guard already alerted by a failed stealth/puzzle check (alarmTriggered):
// higher damage per round (VS-001 §7.3, §3.4).
class AlertedGuardStrategy final : public IGuardBehaviorStrategy
{
  public:
    int decideDamage(const CombatState& state, int nivelAlerta) const override;
};

// Guard in a squad reacting to propagated alert (Issue #220): damage scales
// with how alerted this particular guard is, unlike Standard/Alerted which
// only know about the encounter-wide alarmTriggered flag.
class EscalatingGuardStrategy final : public IGuardBehaviorStrategy
{
  public:
    int decideDamage(const CombatState& state, int nivelAlerta) const override;
};

// Three stateless, reusable strategy instances owned by CombatSystem's
// translation unit (static storage duration) — no per-encounter allocation
// (VS-001 §9.5.3).
struct GuardStrategies
{
    static const IGuardBehaviorStrategy& standard();
    static const IGuardBehaviorStrategy& alerted();
    static const IGuardBehaviorStrategy& escalating();
};

} // namespace cybercba::combat

#endif // CYBERCBA_COMBAT_GUARDBEHAVIORSTRATEGY_H
