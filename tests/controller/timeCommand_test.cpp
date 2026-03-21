#include "timeCommand.hpp"
#include "model/gameModel.hpp"

#include <gtest/gtest.h>

#include <iostream>
#include <sstream>
#include <string>

using namespace CyberpunkCba;

class TimeCommandTest : public ::testing::Test
{
protected:
    std::string captureOutput(Command& cmd)
    {
        std::ostringstream oss;
        std::streambuf* old {std::cout.rdbuf(oss.rdbuf())};
        cmd.execute(m_model);
        std::cout.rdbuf(old);
        return oss.str();
    }

    GameModel m_model {"Ghost_47"};
};

TEST_F(TimeCommandTest, TimeCommand_Name)
{
    TimeCommand cmd;
    EXPECT_EQ(cmd.name(), "time");
}

TEST_F(TimeCommandTest, TimeCommand_Category)
{
    TimeCommand cmd;
    EXPECT_EQ(cmd.category(), "sistema");
}

TEST_F(TimeCommandTest, TimeCommand_DescriptionNotEmpty)
{
    TimeCommand cmd;
    EXPECT_FALSE(cmd.description().empty());
}

TEST_F(TimeCommandTest, TimeCommand_Description_IsExpected)
{
    TimeCommand cmd;
    EXPECT_EQ(cmd.description(), "Devuelve turno y hora.");
}

TEST_F(TimeCommandTest, TimeCommand_Execute_ProducesOutput)
{
    TimeCommand cmd;
    const auto output {captureOutput(cmd)};
    EXPECT_FALSE(output.empty());
}

TEST_F(TimeCommandTest, TimeCommand_Execute_DoesNotModifyModel)
{
    const auto dayBefore {m_model.simulationDay()};
    const auto hourBefore {m_model.currentHour()};
    const auto minuteBefore {m_model.currentMinute()};
    const auto creditsBefore {m_model.credits()};
    const auto alertBefore {m_model.alertLevel()};
    const auto runningBefore {m_model.isRunning()};

    TimeCommand cmd;
    cmd.execute(m_model);

    EXPECT_EQ(m_model.simulationDay(), dayBefore);
    EXPECT_EQ(m_model.currentHour(), hourBefore);
    EXPECT_EQ(m_model.currentMinute(), minuteBefore);
    EXPECT_EQ(m_model.credits(), creditsBefore);
    EXPECT_EQ(m_model.alertLevel(), alertBefore);
    EXPECT_EQ(m_model.isRunning(), runningBefore);
}

TEST_F(TimeCommandTest, TimeCommand_Execute_ContainsMidnightMessage_IfMidnight)
{
    if (!(m_model.currentHour() == 0 && m_model.currentMinute() == 0))
    {
        GTEST_SKIP() << "El modelo no está en 00:00.";
    }

    TimeCommand cmd;
    const auto output {captureOutput(cmd)};
    EXPECT_NE(output.find("Nuevo día de simulación iniciado."), std::string::npos);
}

TEST_F(TimeCommandTest, TimeCommand_Execute_ContainsHoraTurnoAndTiempoRestante_IfNotMidnight)
{
    if (m_model.currentHour() == 0 && m_model.currentMinute() == 0)
    {
        GTEST_SKIP() << "El modelo está en 00:00; este test no aplica.";
    }

    TimeCommand cmd;
    const auto output {captureOutput(cmd)};

    EXPECT_NE(output.find("Hora:"), std::string::npos);
    EXPECT_NE(output.find("Turno:"), std::string::npos);
    EXPECT_NE(output.find("Tiempo restante hasta próximo turno:"), std::string::npos);
}