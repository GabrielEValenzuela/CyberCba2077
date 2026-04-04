#include "common/types.hpp"
#include "scanCommand.hpp"

#include <gtest/gtest.h>

#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "model/gameModel.hpp"

using namespace CyberpunkCba;

class ScanCommandTest : public ::testing::Test
{
protected:
    void setNearbyEntities(GameModel& model, std::vector<WorldEntity> entities)
    {
        auto& nearby {const_cast<std::vector<const WorldEntity*>&>(model.nearbyEntities())};
        nearby.clear();

        m_testEntities = std::move(entities);
        for (const auto& entity : m_testEntities)
        {
            nearby.emplace_back(&entity);
        }
    }

    std::string captureOutput()
    {
        struct CoutRedirectGuard
        {
            explicit CoutRedirectGuard(std::ostream& os, std::streambuf* newBuffer)
                : m_stream(os)
                , m_oldBuffer(os.rdbuf(newBuffer))
            {
            }

            ~CoutRedirectGuard()
            {
                m_stream.rdbuf(m_oldBuffer);
            }

            std::ostream& m_stream;
            std::streambuf* m_oldBuffer;
        };

        std::ostringstream oss;
        CoutRedirectGuard guard {std::cout, oss.rdbuf()};
        Command& commandRef {m_command};
        commandRef.execute(m_model);
        return oss.str();
    }

    GameModel m_model {"Ghost_47"};
    ScanCommand m_command;
    std::vector<WorldEntity> m_testEntities;
};

TEST_F(ScanCommandTest, EntitiesAreSortedByDistance)
{
    setNearbyEntities(m_model,
                      {{"Entidad lejana", EntityDisposition::Neutral, 120},
                       {"Entidad cercana", EntityDisposition::Friendly, 15},
                       {"Entidad media", EntityDisposition::Hostile, 40}});

    const auto output {captureOutput()};
    const auto idxNear {output.find("Entidad cercana")};
    const auto idxMid {output.find("Entidad media")};
    const auto idxFar {output.find("Entidad lejana")};

    ASSERT_NE(idxNear, std::string::npos);
    ASSERT_NE(idxMid, std::string::npos);
    ASSERT_NE(idxFar, std::string::npos);
    EXPECT_LT(idxNear, idxMid);
    EXPECT_LT(idxMid, idxFar);
}

TEST_F(ScanCommandTest, AlertDoesNotChangeWhenAlreadyAtExpectedLevel)
{
    setNearbyEntities(m_model, {{"Corporativo Militech", EntityDisposition::Hostile, 25}});
    const auto before {m_model.alertLevel()};
    const auto beforeCause {m_model.lastAlertCause()};

    captureOutput();

    EXPECT_EQ(m_model.alertLevel(), before);
    EXPECT_EQ(m_model.lastAlertCause(), beforeCause);
}

TEST_F(ScanCommandTest, EmptyZoneDoesNotModifyModel)
{
    setNearbyEntities(m_model, {});
    const auto beforeAlert {m_model.alertLevel()};
    const auto beforeCause {m_model.lastAlertCause()};

    const auto output {captureOutput()};

    EXPECT_EQ(m_model.alertLevel(), beforeAlert);
    EXPECT_EQ(m_model.lastAlertCause(), beforeCause);
    EXPECT_NE(output.find("Sin entidades detectadas."), std::string::npos);
}

TEST_F(ScanCommandTest, OnlyFriendlyDoesNotIncrementAlert)
{
    setNearbyEntities(
        m_model,
        {{"Mercader aliado", EntityDisposition::Friendly, 18}, {"Netrunner amigo", EntityDisposition::Friendly, 42}});
    const auto beforeAlert {m_model.alertLevel()};
    const auto beforeCause {m_model.lastAlertCause()};

    captureOutput();

    EXPECT_EQ(m_model.alertLevel(), beforeAlert);
    EXPECT_EQ(m_model.lastAlertCause(), beforeCause);
}

TEST_F(ScanCommandTest, OutputContainsEachDisposition)
{
    setNearbyEntities(m_model,
                      {{"Aliado", EntityDisposition::Friendly, 10},
                       {"Civil", EntityDisposition::Neutral, 20},
                       {"Guardia", EntityDisposition::Hostile, 30}});

    const auto output {captureOutput()};

    EXPECT_NE(output.find("[AMIGABLE]"), std::string::npos);
    EXPECT_NE(output.find("[NEUTRAL]"), std::string::npos);
    EXPECT_NE(output.find("[HOSTIL]"), std::string::npos);
}

TEST_F(ScanCommandTest, ScanRaisesAlertUpToTargetByHostileCount)
{
    setNearbyEntities(m_model,
                      {{"Hostile A", EntityDisposition::Hostile, 10},
                       {"Hostile B", EntityDisposition::Hostile, 20},
                       {"Hostile C", EntityDisposition::Hostile, 30}});

    captureOutput();

    EXPECT_EQ(m_model.alertLevel(), AlertLevel::High);
    EXPECT_FALSE(m_model.lastAlertCause().empty());
}

TEST_F(ScanCommandTest, ScanSaturatesAtMaximumWhenHostilesAreFourOrMore)
{
    setNearbyEntities(m_model,
                      {{"Hostile A", EntityDisposition::Hostile, 10},
                       {"Hostile B", EntityDisposition::Hostile, 20},
                       {"Hostile C", EntityDisposition::Hostile, 30},
                       {"Hostile D", EntityDisposition::Hostile, 40},
                       {"Hostile E", EntityDisposition::Hostile, 50}});

    captureOutput();

    EXPECT_EQ(m_model.alertLevel(), AlertLevel::Maximum);
}

TEST_F(ScanCommandTest, ScanDoesNotDecreaseAlertWhenHostileCountDrops)
{
    m_model.incrementAlert("Escalada previa");
    m_model.incrementAlert("Escalada previa");
    EXPECT_EQ(m_model.alertLevel(), AlertLevel::High);

    setNearbyEntities(m_model, {{"Civil", EntityDisposition::Neutral, 15}});
    const auto beforeCause {m_model.lastAlertCause()};

    captureOutput();

    EXPECT_EQ(m_model.alertLevel(), AlertLevel::High);
    EXPECT_EQ(m_model.lastAlertCause(), beforeCause);
}
