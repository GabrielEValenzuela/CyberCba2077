#include "cybercba/combat/GuardBehaviorStrategy.hpp"

#include <gtest/gtest.h>

namespace cybercba::combat
{
namespace
{

TEST(GuardBehaviorStrategyTest, StandardStrategyDamageIsFixed)
{
    const CombatState state{30, 15};
    EXPECT_EQ(GuardStrategies::standard().decideDamage(state), 8);
}

TEST(GuardBehaviorStrategyTest, AlertedStrategyDamageIsHigherThanStandard)
{
    const CombatState state{30, 15};
    EXPECT_GT(GuardStrategies::alerted().decideDamage(state), GuardStrategies::standard().decideDamage(state));
}

} // namespace
} // namespace cybercba::combat
