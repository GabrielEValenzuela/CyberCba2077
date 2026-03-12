#include "common/types.hpp"
#include "model/gameModel.hpp"

#include <gtest/gtest.h>

using namespace CyberpunkCba;

// =============================================================================
// Fixture
// =============================================================================

class GameModelTest : public ::testing::Test
{
protected:
    GameModel m_model {"Ghost_47"};
};

// =============================================================================
// Construcción e invariantes iniciales
// =============================================================================

TEST_F(GameModelTest, InitialState_PlayerName)
{
    EXPECT_EQ(m_model.playerName(), "Ghost_47");
}

TEST_F(GameModelTest, InitialState_HpIsMax)
{
    EXPECT_EQ(m_model.hp(), m_model.maxHp());
    EXPECT_GT(m_model.maxHp(), 0);
}

TEST_F(GameModelTest, InitialState_CreditsPositive)
{
    EXPECT_GT(m_model.credits(), 0);
}

TEST_F(GameModelTest, InitialState_IsRunning)
{
    EXPECT_TRUE(m_model.isRunning());
}

TEST_F(GameModelTest, InitialState_CommandCountZero)
{
    EXPECT_EQ(m_model.commandCount(), 0);
}

TEST_F(GameModelTest, InitialState_HackAttemptsAtMax)
{
    EXPECT_EQ(m_model.hackAttempts(), m_model.maxHackAttempts());
    EXPECT_GT(m_model.maxHackAttempts(), 0);
}

TEST_F(GameModelTest, InitialState_InventoryNotEmpty)
{
    // Tiene al menos el ítem inicial
    EXPECT_FALSE(m_model.inventory().empty());
}

TEST_F(GameModelTest, InitialState_HasActiveMission)
{
    EXPECT_NE(m_model.activeMission(), nullptr);
}

TEST_F(GameModelTest, InitialState_CurrentZoneNotEmpty)
{
    EXPECT_FALSE(m_model.currentZone().empty());
}

TEST_F(GameModelTest, InitialState_AdjacentZonesNotEmpty)
{
    EXPECT_FALSE(m_model.adjacentZones().empty());
}

// =============================================================================
// isCriticalHp
// =============================================================================

TEST_F(GameModelTest, IsCriticalHp_FullHp_IsFalse)
{
    EXPECT_FALSE(m_model.isCriticalHp());
}

// =============================================================================
// incrementAlert
// =============================================================================

TEST_F(GameModelTest, IncrementAlert_ChangesLevel)
{
    const auto before {m_model.alertLevel()};
    m_model.incrementAlert("test scan");
    EXPECT_NE(m_model.alertLevel(), before);
}

TEST_F(GameModelTest, IncrementAlert_UpdatesCause)
{
    m_model.incrementAlert("causa test");
    EXPECT_EQ(m_model.lastAlertCause(), "causa test");
}

TEST_F(GameModelTest, IncrementAlert_SaturatesAtMaximum)
{
    for (int i {}; i < 10; ++i)
    {
        m_model.incrementAlert("spam");
    }
    EXPECT_EQ(m_model.alertLevel(), AlertLevel::Maximum);
}

// =============================================================================
// spendCredits
// =============================================================================

TEST_F(GameModelTest, SpendCredits_DeductsCorrectly)
{
    const auto before {m_model.credits()};
    m_model.spendCredits(50);
    EXPECT_EQ(m_model.credits(), before - 50);
}

TEST_F(GameModelTest, SpendCredits_NeverGoesNegative)
{
    const auto allCredits {m_model.credits()};
    m_model.spendCredits(allCredits);
    EXPECT_EQ(m_model.credits(), 0);
}

// =============================================================================
// logAction
// =============================================================================

TEST_F(GameModelTest, LogAction_AddsEntry)
{
    const auto before {m_model.actionLog().size()};
    m_model.logAction("evento test");
    EXPECT_EQ(m_model.actionLog().size(), before + 1);
}

TEST_F(GameModelTest, LogAction_MessagePreserved)
{
    m_model.logAction("mensaje especifico");
    EXPECT_EQ(m_model.actionLog().back().message, "mensaje especifico");
}

// =============================================================================
// incrementCommandCount
// =============================================================================

TEST_F(GameModelTest, IncrementCommandCount_IsMonotone)
{
    const auto before {m_model.commandCount()};
    m_model.incrementCommandCount();
    m_model.incrementCommandCount();
    EXPECT_EQ(m_model.commandCount(), before + 2);
}

// =============================================================================
// consumeHackAttempt
// =============================================================================

TEST_F(GameModelTest, ConsumeHackAttempt_Decrements)
{
    const auto before {m_model.hackAttempts()};
    m_model.consumeHackAttempt();
    EXPECT_EQ(m_model.hackAttempts(), before - 1);
}

// =============================================================================
// quit
// =============================================================================

TEST_F(GameModelTest, Quit_SetsRunningFalse)
{
    m_model.quit();
    EXPECT_FALSE(m_model.isRunning());
}

TEST_F(GameModelTest, Quit_IsIdempotent)
{
    m_model.quit();
    m_model.quit(); // segunda llamada — no-op, no crash
    EXPECT_FALSE(m_model.isRunning());
}

// =============================================================================
// repValue y dominantFaction
// =============================================================================

TEST_F(GameModelTest, RepValue_InRange)
{
    for (uint8_t i {}; i < FACTION_COUNT; ++i)
    {
        const auto val {m_model.repValue(static_cast<Faction>(i))};
        EXPECT_GE(val, 0);
        EXPECT_LE(val, 100);
    }
}

TEST_F(GameModelTest, DominantFaction_ReturnsValid)
{
    const auto dominant {m_model.dominantFaction()};
    const auto idx {static_cast<uint8_t>(dominant)};
    EXPECT_LT(idx, FACTION_COUNT);
}

// =============================================================================
// nearbyEntities — observadores
// =============================================================================

TEST_F(GameModelTest, NearbyEntities_NoNullptrs)
{
    for (const auto* pEntity : m_model.nearbyEntities())
    {
        EXPECT_NE(pEntity, nullptr);
    }
}

TEST_F(GameModelTest, NearbyEntities_NamesNotEmpty)
{
    for (const auto* pEntity : m_model.nearbyEntities())
    {
        EXPECT_FALSE(pEntity->name.empty());
    }
}

// =============================================================================
// sessionDuration
// =============================================================================

TEST_F(GameModelTest, SessionDuration_NonNegative)
{
    EXPECT_GE(m_model.sessionDuration().count(), 0);
}
