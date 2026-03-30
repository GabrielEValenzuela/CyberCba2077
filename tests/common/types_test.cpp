#include "common/types.hpp"

#include <gtest/gtest.h>

using namespace CyberpunkCba;

// =============================================================================
// alertLevelToString
// =============================================================================

TEST(AlertLevelTest, ToString_AllValues)
{
    EXPECT_EQ(alertLevelToString(AlertLevel::None), "NINGUNA");
    EXPECT_EQ(alertLevelToString(AlertLevel::Low), "BAJA");
    EXPECT_EQ(alertLevelToString(AlertLevel::Medium), "MEDIA");
    EXPECT_EQ(alertLevelToString(AlertLevel::High), "ALTA");
    EXPECT_EQ(alertLevelToString(AlertLevel::Maximum), "MÁXIMA");
}

TEST(AlertLevelTest, ToString_NeverEmpty)
{
    EXPECT_FALSE(alertLevelToString(AlertLevel::None).empty());
    EXPECT_FALSE(alertLevelToString(AlertLevel::Maximum).empty());
}

// =============================================================================
// incrementAlertLevel
// =============================================================================

TEST(AlertLevelTest, Increment_NoneToLow)
{
    EXPECT_EQ(incrementAlertLevel(AlertLevel::None), AlertLevel::Low);
}

TEST(AlertLevelTest, Increment_LowToMedium)
{
    EXPECT_EQ(incrementAlertLevel(AlertLevel::Low), AlertLevel::Medium);
}

TEST(AlertLevelTest, Increment_MediumToHigh)
{
    EXPECT_EQ(incrementAlertLevel(AlertLevel::Medium), AlertLevel::High);
}

TEST(AlertLevelTest, Increment_HighToMaximum)
{
    EXPECT_EQ(incrementAlertLevel(AlertLevel::High), AlertLevel::Maximum);
}

TEST(AlertLevelTest, Increment_MaximumSaturates)
{
    EXPECT_EQ(incrementAlertLevel(AlertLevel::Maximum), AlertLevel::Maximum);
}

// =============================================================================
// timeOfDayToString
// =============================================================================

TEST(TimeOfDayTest, ToString_AllValues)
{
    EXPECT_EQ(timeOfDayToString(TimeOfDay::Dawn), "AMANECER");
    EXPECT_EQ(timeOfDayToString(TimeOfDay::Day), "DÍA");
    EXPECT_EQ(timeOfDayToString(TimeOfDay::Dusk), "ATARDECER");
    EXPECT_EQ(timeOfDayToString(TimeOfDay::Night), "NOCHE");
}

// =============================================================================
// timeOfDayFromHour
// =============================================================================

TEST(TimeOfDayTest, FromHour_Dawn)
{
    EXPECT_EQ(timeOfDayFromHour(4), TimeOfDay::Dawn);
    EXPECT_EQ(timeOfDayFromHour(5), TimeOfDay::Dawn);
}

TEST(TimeOfDayTest, FromHour_Day)
{
    EXPECT_EQ(timeOfDayFromHour(6), TimeOfDay::Day);
    EXPECT_EQ(timeOfDayFromHour(12), TimeOfDay::Day);
    EXPECT_EQ(timeOfDayFromHour(17), TimeOfDay::Day);
}

TEST(TimeOfDayTest, FromHour_Dusk)
{
    EXPECT_EQ(timeOfDayFromHour(18), TimeOfDay::Dusk);
    EXPECT_EQ(timeOfDayFromHour(19), TimeOfDay::Dusk);
}

TEST(TimeOfDayTest, FromHour_Night)
{
    EXPECT_EQ(timeOfDayFromHour(0), TimeOfDay::Night);
    EXPECT_EQ(timeOfDayFromHour(20), TimeOfDay::Night);
    EXPECT_EQ(timeOfDayFromHour(23), TimeOfDay::Night);
    EXPECT_EQ(timeOfDayFromHour(3), TimeOfDay::Night);
}

// =============================================================================
// minutesUntilNextTurn
// =============================================================================

TEST(TimeOfDayTest, MinutesUntilNextTurn_DawnBoundary)
{
    // A las 04:00 faltan 120 minutos para Day (06:00)
    EXPECT_EQ(minutesUntilNextTurn(4, 0), 120);
}

TEST(TimeOfDayTest, MinutesUntilNextTurn_DayBoundary)
{
    // A las 06:00 faltan 720 minutos para Dusk (18:00)
    EXPECT_EQ(minutesUntilNextTurn(6, 0), 720);
}

TEST(TimeOfDayTest, MinutesUntilNextTurn_DuskBoundary)
{
    // A las 18:00 faltan 120 minutos para Night (20:00)
    EXPECT_EQ(minutesUntilNextTurn(18, 0), 120);
}

TEST(TimeOfDayTest, MinutesUntilNextTurn_Midnight)
{
    // A las 00:00 faltan 240 minutos para Dawn (04:00)
    EXPECT_EQ(minutesUntilNextTurn(0, 0), 360);
}

TEST(TimeOfDayTest, MinutesUntilNextTurn_NeverZero)
{
    // En cualquier momento siempre queda algo
    EXPECT_GT(minutesUntilNextTurn(12, 30), 0);
    EXPECT_GT(minutesUntilNextTurn(0, 0), 0);
    EXPECT_GT(minutesUntilNextTurn(23, 59), 0);
}

// =============================================================================
// factionToString
// =============================================================================

TEST(FactionTest, ToString_AllValues)
{
    EXPECT_EQ(factionToString(Faction::Corporations), "CORPORACIONES");
    EXPECT_EQ(factionToString(Faction::Streeters), "CALLEJEROS");
    EXPECT_EQ(factionToString(Faction::NetRunners), "NETRUNNERS");
}

// =============================================================================
// repLevelToString + repLevelFromValue
// =============================================================================

TEST(RepLevelTest, ToString_AllValues)
{
    EXPECT_EQ(repLevelToString(RepLevel::Hostile), "HOSTIL");
    EXPECT_EQ(repLevelToString(RepLevel::Neutral), "NEUTRAL");
    EXPECT_EQ(repLevelToString(RepLevel::Allied), "ALIADO");
    EXPECT_EQ(repLevelToString(RepLevel::Legendary), "LEGENDARIO");
}

TEST(RepLevelTest, FromValue_Hostile)
{
    EXPECT_EQ(repLevelFromValue(0), RepLevel::Hostile);
    EXPECT_EQ(repLevelFromValue(29), RepLevel::Hostile);
}

TEST(RepLevelTest, FromValue_Neutral)
{
    EXPECT_EQ(repLevelFromValue(30), RepLevel::Neutral);
    EXPECT_EQ(repLevelFromValue(59), RepLevel::Neutral);
}

TEST(RepLevelTest, FromValue_Allied)
{
    EXPECT_EQ(repLevelFromValue(60), RepLevel::Allied);
    EXPECT_EQ(repLevelFromValue(84), RepLevel::Allied);
}

TEST(RepLevelTest, FromValue_Legendary)
{
    EXPECT_EQ(repLevelFromValue(85), RepLevel::Legendary);
    EXPECT_EQ(repLevelFromValue(100), RepLevel::Legendary);
}

TEST(RepLevelTest, FromValue_Boundaries)
{
    // Bordes exactos de cada transición
    EXPECT_NE(repLevelFromValue(29), repLevelFromValue(30));
    EXPECT_NE(repLevelFromValue(59), repLevelFromValue(60));
    EXPECT_NE(repLevelFromValue(84), repLevelFromValue(85));
}

// =============================================================================
// missionStatusToString
// =============================================================================

TEST(MissionStatusTest, ToString_AllValues)
{
    EXPECT_EQ(missionStatusToString(MissionStatus::Pending), "PENDIENTE");
    EXPECT_EQ(missionStatusToString(MissionStatus::InProgress), "EN CURSO");
    EXPECT_EQ(missionStatusToString(MissionStatus::Completed), "COMPLETADA");
    EXPECT_EQ(missionStatusToString(MissionStatus::Failed), "FALLIDA");
}

// =============================================================================
// itemTypeToString
// =============================================================================

TEST(ItemTypeTest, ToString_AllValues)
{
    EXPECT_EQ(itemTypeToString(ItemType::Weapon), "ARMA");
    EXPECT_EQ(itemTypeToString(ItemType::Consumable), "CONSUMIBLE");
    EXPECT_EQ(itemTypeToString(ItemType::Value), "VALOR");
    EXPECT_EQ(itemTypeToString(ItemType::Tech), "TECH");
}

// =============================================================================
// entityDispositionToString
// =============================================================================

TEST(EntityDispositionTest, ToString_AllValues)
{
    EXPECT_EQ(entityDispositionToString(EntityDisposition::Friendly), "AMIGABLE");
    EXPECT_EQ(entityDispositionToString(EntityDisposition::Neutral), "NEUTRAL");
    EXPECT_EQ(entityDispositionToString(EntityDisposition::Hostile), "HOSTIL");
}
