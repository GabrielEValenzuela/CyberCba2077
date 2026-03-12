#include "common/types.hpp"
#include "model/gameModel.hpp"
#include <algorithm>

#include <gtest/gtest.h>

using namespace CyberpunkCba;

class GameModelGettersTest : public ::testing::Test
{
protected:
    GameModel m_model {"Ghost_47"};
};

// =============================================================================
// Mundo — getters sin cubrir
// =============================================================================

TEST_F(GameModelGettersTest, SimulationDay_Initial)
{
    EXPECT_GE(m_model.simulationDay(), 1);
}

TEST_F(GameModelGettersTest, CurrentHour_InRange)
{
    EXPECT_GE(m_model.currentHour(), 0);
    EXPECT_LE(m_model.currentHour(), 23);
}

TEST_F(GameModelGettersTest, CurrentMinute_InRange)
{
    EXPECT_GE(m_model.currentMinute(), 0);
    EXPECT_LE(m_model.currentMinute(), 59);
}

TEST_F(GameModelGettersTest, TimeOfDay_DerivedFromHour)
{
    const auto expected {timeOfDayFromHour(m_model.currentHour())};
    EXPECT_EQ(m_model.timeOfDay(), expected);
}

TEST_F(GameModelGettersTest, TotalZoneCount_Positive)
{
    EXPECT_GT(m_model.totalZoneCount(), 0);
}

TEST_F(GameModelGettersTest, VisitedZones_ContainsCurrentZone)
{
    const auto& visited {m_model.visitedZones()};
    const auto it {std::find(visited.begin(), visited.end(), m_model.currentZone())};
    EXPECT_NE(it, visited.end());
}

TEST_F(GameModelGettersTest, LastAlertCause_InitiallyEmpty)
{
    // Sin incrementAlert llamado, la causa debe ser vacía
    EXPECT_TRUE(m_model.lastAlertCause().empty());
}

// =============================================================================
// Inventario — getters sin cubrir
// =============================================================================

TEST_F(GameModelGettersTest, InventoryCapacity_Positive)
{
    EXPECT_GT(m_model.inventoryCapacity(), 0);
}

TEST_F(GameModelGettersTest, IsInventoryFull_InitiallyFalse)
{
    EXPECT_FALSE(m_model.isInventoryFull());
}

TEST_F(GameModelGettersTest, Inventory_SizeWithinCapacity)
{
    EXPECT_LE(static_cast<int>(m_model.inventory().size()), m_model.inventoryCapacity());
}

// =============================================================================
// Reputación — dominantFaction con empate
// =============================================================================

TEST_F(GameModelGettersTest, DominantFaction_WithEqualRep_ReturnsLowestIndex)
{
    // Estado inicial: todas las facciones tienen el mismo rep (INITIAL_REP)
    // dominantFaction() debe retornar Corporations (índice 0)
    EXPECT_EQ(m_model.dominantFaction(), Faction::Corporations);
}

TEST_F(GameModelGettersTest, RepValue_AllFactionsInRange)
{
    for (uint8_t i {}; i < FACTION_COUNT; ++i)
    {
        const auto val {m_model.repValue(static_cast<Faction>(i))};
        EXPECT_GE(val, 0);
        EXPECT_LE(val, 100);
    }
}

// =============================================================================
// Misiones — getters con nullptr
// =============================================================================

TEST_F(GameModelGettersTest, ActiveMission_InitiallyNotNull)
{
    EXPECT_NE(m_model.activeMission(), nullptr);
}

TEST_F(GameModelGettersTest, ActiveMission_NameNotEmpty)
{
    ASSERT_NE(m_model.activeMission(), nullptr);
    EXPECT_FALSE(m_model.activeMission()->name.empty());
}

TEST_F(GameModelGettersTest, LastFailedMission_InitiallyNull)
{
    EXPECT_EQ(m_model.lastFailedMission(), nullptr);
}

TEST_F(GameModelGettersTest, CompletedMissions_InitiallyZero)
{
    EXPECT_EQ(m_model.completedMissions(), 0);
}

TEST_F(GameModelGettersTest, FailedMissions_InitiallyZero)
{
    EXPECT_EQ(m_model.failedMissions(), 0);
}

// =============================================================================
// Hackeo — getters sin cubrir
// =============================================================================

TEST_F(GameModelGettersTest, HackCost_Positive)
{
    EXPECT_GT(m_model.hackCost(), 0);
}

TEST_F(GameModelGettersTest, MaxHackAttempts_Positive)
{
    EXPECT_GT(m_model.maxHackAttempts(), 0);
}

TEST_F(GameModelGettersTest, HackAttempts_InitiallyAtMax)
{
    EXPECT_EQ(m_model.hackAttempts(), m_model.maxHackAttempts());
}

// =============================================================================
// Sesión — getters sin cubrir
// =============================================================================

TEST_F(GameModelGettersTest, ActionLog_InitiallyNotEmpty)
{
    // El constructor registra al menos un evento
    EXPECT_FALSE(m_model.actionLog().empty());
}

TEST_F(GameModelGettersTest, ActionLog_FirstEntry_HourInRange)
{
    const auto& entry {m_model.actionLog().front()};
    // hour puede ser -1 (no disponible) o en [0, 23]
    EXPECT_TRUE(entry.hour == -1 || (entry.hour >= 0 && entry.hour <= 23));
}

TEST_F(GameModelGettersTest, ActionLog_FirstEntry_MessageNotEmpty)
{
    EXPECT_FALSE(m_model.actionLog().front().message.empty());
}
