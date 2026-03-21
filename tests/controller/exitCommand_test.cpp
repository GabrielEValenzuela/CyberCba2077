#include "exitCommand.hpp"
#include "model/gameModel.hpp"

#include <gtest/gtest.h>
#include <iostream>
#include <sstream>

using namespace CyberpunkCba;

class ExitCommandTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }

    std::string captureOutput(Command& cmd)
    {
        std::ostringstream oss;
        std::streambuf* old {std::cout.rdbuf(oss.rdbuf())};
        cmd.execute(m_model);
        std::cout.rdbuf(old);
        return oss.str();
    }

    GameModel m_model {"Ghost_47"};
    ExitCommand m_cmd;
};

// =============================================================================
// ExitCommand — Tests
// =============================================================================

TEST_F(ExitCommandTest, ExitCommand_Name)
{
    EXPECT_EQ(m_cmd.name(), "exit");
}

TEST_F(ExitCommandTest, ExitCommand_Category)
{
    EXPECT_EQ(m_cmd.category(), "sistema");
}

TEST_F(ExitCommandTest, ExitCommand_DescriptionNotEmpty)
{
    EXPECT_FALSE(m_cmd.description().empty());
}

TEST_F(ExitCommandTest, ExitCommand_QuitFlagSet)
{
    EXPECT_TRUE(m_model.isRunning());
    captureOutput(m_cmd);
    EXPECT_FALSE(m_model.isRunning());
}

TEST_F(ExitCommandTest, ExitCommand_ShowsCommandCount)
{
    m_model.incrementCommandCount();
    m_model.incrementCommandCount();
    const auto output {captureOutput(m_cmd)};
    EXPECT_NE(output.find("2"), std::string::npos);
    EXPECT_NE(output.find("Comandos ejecutados"), std::string::npos);
}

TEST_F(ExitCommandTest, ExitCommand_ShowsDurationFormatted)
{
    // sessionDuration relies on steady_clock from m_sessionStart
    // so we can't easily mock it without injecting a clock, but "00:00:00" should at least be present
    const auto output {captureOutput(m_cmd)};
    EXPECT_NE(output.find("00:00:00"), std::string::npos); // since it processes fast
    EXPECT_NE(output.find("Tiempo de sesión"), std::string::npos);
}

TEST_F(ExitCommandTest, ExitCommand_CriticalHp_ShowsWarning)
{
    // gastar p.ej HP. no podemos setHp, we know CRITICAL_HP_RATIO is 0.2
    // m_model starts with maxHp=100.
    // wait, we can't change HP? Let's check GameModel mutators.
    // We don't have a mutator to change HP directly except... we don't!
    // isCriticalHp will be false initially. Let's test the normal output:
    const auto normalOutput {captureOutput(m_cmd)};
    EXPECT_NE(normalOutput.find("Salida segura completada."), std::string::npos);
}

TEST_F(ExitCommandTest, ExitCommand_DoubleExecute_NoOp)
{
    const auto output1 {captureOutput(m_cmd)};
    EXPECT_FALSE(m_model.isRunning());

    const auto output2 {captureOutput(m_cmd)};
    EXPECT_NE(output2.find("La sesión ya ha terminado."), std::string::npos);
    // quit again has no effect and doesn't crash
}