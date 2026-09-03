#include "cybercba/combat/GuardBehaviorStrategy.hpp"

#include <gtest/gtest.h>

namespace cybercba::combat
{
namespace
{

TEST(GuardBehaviorStrategyTest, StandardStrategyDamageIsFixed)
{
    const CombatState state{30, 15};
    EXPECT_EQ(GuardStrategies::standard().decideDamage(state, 0), 8);
}

TEST(GuardBehaviorStrategyTest, AlertedStrategyDamageIsHigherThanStandard)
{
    const CombatState state{30, 15};
    EXPECT_GT(GuardStrategies::alerted().decideDamage(state, 0), GuardStrategies::standard().decideDamage(state, 0));
}

TEST(GuardBehaviorStrategyTest, EscalatingStrategyIgnoresAlarmTriggeredFlag)
{
    // Standard/Alerted key off CombatState::alarmTriggered; Escalating keys
    // off nivelAlerta instead — changing the flag alone must not move it.
    CombatState state{30, 15};
    const int withoutAlarm = GuardStrategies::escalating().decideDamage(state, 3);
    state.alarmTriggered   = true;
    const int withAlarm    = GuardStrategies::escalating().decideDamage(state, 3);
    EXPECT_EQ(withoutAlarm, withAlarm);
}

TEST(GuardBehaviorStrategyTest, EscalatingStrategyDamageGrowsWithAlertLevel)
{
    const CombatState state{30, 15};
    const int lowAlert  = GuardStrategies::escalating().decideDamage(state, 1);
    const int highAlert = GuardStrategies::escalating().decideDamage(state, 3);
    EXPECT_GT(highAlert, lowAlert);
}

TEST(GuardBehaviorStrategyTest, EscalatingStrategyAtLowestAlertBeatsAlerted)
{
    // The weakest propagated alert (level 1) should already read as "more
    // dangerous" than the old binary Alerted strategy.
    const CombatState state{30, 15};
    EXPECT_GT(GuardStrategies::escalating().decideDamage(state, 1), GuardStrategies::alerted().decideDamage(state, 0));
}

} // namespace
} // namespace cybercba::combat
