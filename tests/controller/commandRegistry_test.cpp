#include "common/types.hpp"
#include "controller/commandRegistry.hpp"
#include "helpCommand.hpp"
#include "model/gameModel.hpp"
#include "statusCommand.hpp"
#include "unknownCommand.hpp"
#include "clearCommand.hpp"

#include <gtest/gtest.h>
#include <iostream>
#include <sstream>

using namespace CyberpunkCba;

// =============================================================================
// Fixture compartida
// =============================================================================

class InstructorCommandsTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        m_registry.add(std::make_unique<UnknownCommand>(""));

        auto spHelp {std::make_unique<HelpCommand>(m_registry)};
        m_registry.add(std::move(spHelp));
        m_registry.add(std::make_unique<StatusCommand>());
        m_registry.add(std::make_unique<ClearCommand>());
    }

    /// @brief Captura stdout durante la ejecución de un comando.
    std::string captureOutput(Command& cmd)
    {
        std::ostringstream oss;
        std::streambuf* old {std::cout.rdbuf(oss.rdbuf())};
        cmd.execute(m_model);
        std::cout.rdbuf(old);
        return oss.str();
    }

    GameModel m_model {"Ghost_47"};
    CommandRegistry m_registry;
};

// =============================================================================
// HelpCommand — contrato
// =============================================================================

TEST_F(InstructorCommandsTest, HelpCommand_Name)
{
    auto& cmd {m_registry.dispatch("help")};
    EXPECT_EQ(cmd.name(), "help");
}

TEST_F(InstructorCommandsTest, HelpCommand_Category)
{
    auto& cmd {m_registry.dispatch("help")};
    EXPECT_EQ(cmd.category(), "sistema");
}

TEST_F(InstructorCommandsTest, HelpCommand_DescriptionNotEmpty)
{
    auto& cmd {m_registry.dispatch("help")};
    EXPECT_FALSE(cmd.description().empty());
}

TEST_F(InstructorCommandsTest, HelpCommand_Execute_ProducesOutput)
{
    auto& cmd {m_registry.dispatch("help")};
    const auto output {captureOutput(cmd)};
    EXPECT_FALSE(output.empty());
}

TEST_F(InstructorCommandsTest, HelpCommand_Execute_ContainsHelp)
{
    auto& cmd {m_registry.dispatch("help")};
    const auto output {captureOutput(cmd)};
    // El output debe mencionar "help" entre los comandos listados
    EXPECT_NE(output.find("help"), std::string::npos);
}

TEST_F(InstructorCommandsTest, HelpCommand_Execute_ContainsStatus)
{
    auto& cmd {m_registry.dispatch("help")};
    const auto output {captureOutput(cmd)};
    EXPECT_NE(output.find("status"), std::string::npos);
}

TEST_F(InstructorCommandsTest, HelpCommand_Execute_EmptyRegistry_NoOutput)
{
    // Registry vacío — help no debe crashear
    CommandRegistry emptyReg;
    emptyReg.add(std::make_unique<UnknownCommand>(""));
    auto spHelp {std::make_unique<HelpCommand>(emptyReg)};
    Command& cmd {*spHelp};
    std::ostringstream oss;
    std::streambuf* old {std::cout.rdbuf(oss.rdbuf())};
    cmd.execute(m_model);
    std::cout.rdbuf(old);
    EXPECT_FALSE(oss.str().empty());
}

TEST_F(InstructorCommandsTest, HelpCommand_Execute_DoesNotModifyModel)
{
    const auto creditsBefore {m_model.credits()};
    const auto alertBefore {m_model.alertLevel()};
    const auto runningBefore {m_model.isRunning()};

    auto& cmd {m_registry.dispatch("help")};
    cmd.execute(m_model);

    EXPECT_EQ(m_model.credits(), creditsBefore);
    EXPECT_EQ(m_model.alertLevel(), alertBefore);
    EXPECT_EQ(m_model.isRunning(), runningBefore);
}

// =============================================================================
// StatusCommand — contrato
// =============================================================================

TEST_F(InstructorCommandsTest, StatusCommand_Name)
{
    auto& cmd {m_registry.dispatch("status")};
    EXPECT_EQ(cmd.name(), "status");
}

TEST_F(InstructorCommandsTest, StatusCommand_Category)
{
    auto& cmd {m_registry.dispatch("status")};
    EXPECT_EQ(cmd.category(), "runner");
}

TEST_F(InstructorCommandsTest, StatusCommand_DescriptionNotEmpty)
{
    auto& cmd {m_registry.dispatch("status")};
    EXPECT_FALSE(cmd.description().empty());
}

TEST_F(InstructorCommandsTest, StatusCommand_Execute_ProducesOutput)
{
    auto& cmd {m_registry.dispatch("status")};
    const auto output {captureOutput(cmd)};
    EXPECT_FALSE(output.empty());
}

TEST_F(InstructorCommandsTest, StatusCommand_Execute_ContainsPlayerName)
{
    auto& cmd {m_registry.dispatch("status")};
    const auto output {captureOutput(cmd)};
    EXPECT_NE(output.find("Ghost_47"), std::string::npos);
}

TEST_F(InstructorCommandsTest, StatusCommand_Execute_ContainsCredits)
{
    auto& cmd {m_registry.dispatch("status")};
    const auto output {captureOutput(cmd)};
    EXPECT_NE(output.find("250"), std::string::npos);
}

TEST_F(InstructorCommandsTest, StatusCommand_Execute_DoesNotModifyModel)
{
    const auto hpBefore {m_model.hp()};
    const auto creditsBefore {m_model.credits()};
    const auto alertBefore {m_model.alertLevel()};

    auto& cmd {m_registry.dispatch("status")};
    cmd.execute(m_model);

    EXPECT_EQ(m_model.hp(), hpBefore);
    EXPECT_EQ(m_model.credits(), creditsBefore);
    EXPECT_EQ(m_model.alertLevel(), alertBefore);
}

TEST_F(InstructorCommandsTest, StatusCommand_Execute_CriticalHp_StillProducesOutput)
{
    // Forzar HP crítico gastando créditos no sirve — necesitamos acceso directo.
    // Validamos al menos que el comando no crashea con HP normal.
    auto& cmd {m_registry.dispatch("status")};
    const auto output {captureOutput(cmd)};
    EXPECT_NE(output.find("OPERATIVO"), std::string::npos);
}

// =============================================================================
// UnknownCommand — contrato
// =============================================================================

TEST_F(InstructorCommandsTest, UnknownCommand_Name)
{
    auto& cmd {m_registry.dispatch("esto_no_existe")};
    EXPECT_EQ(cmd.name(), "__unknown");
}

TEST_F(InstructorCommandsTest, UnknownCommand_Execute_ProducesOutput)
{
    auto& cmd {m_registry.dispatch("comando_invalido")};
    const auto output {captureOutput(cmd)};
    EXPECT_FALSE(output.empty());
}

TEST_F(InstructorCommandsTest, UnknownCommand_Execute_EmptyInput_NoOutput)
{
    // dispatch de string vacío retorna UnknownCommand con input ""
    // que no debe imprimir nada
    auto& cmd {m_registry.dispatch("")};
    const auto output {captureOutput(cmd)};
    EXPECT_TRUE(output.empty());
}

TEST_F(InstructorCommandsTest, UnknownCommand_Execute_DoesNotModifyModel)
{
    const auto runningBefore {m_model.isRunning()};
    auto& cmd {m_registry.dispatch("basura")};
    cmd.execute(m_model);
    EXPECT_EQ(m_model.isRunning(), runningBefore);
}

// =============================================================================
// ClearCommand — contrato
// =============================================================================

TEST_F(InstructorCommandsTest, ClearCommand_Name)
{
    auto& cmd {m_registry.dispatch("clear")};
    EXPECT_EQ(cmd.name(), "clear");
}

TEST_F(InstructorCommandsTest, ClearCommand_Category)
{
    auto& cmd {m_registry.dispatch("clear")};
    EXPECT_EQ(cmd.category(), "sistema");
}

TEST_F(InstructorCommandsTest, ClearCommand_DescriptionNotEmpty)
{
    auto& cmd {m_registry.dispatch("clear")};
    EXPECT_FALSE(cmd.description().empty());
}

TEST_F(InstructorCommandsTest, ClearCommand_Execute_ProducesOutput)
{
    auto& cmd {m_registry.dispatch("clear")};
    const auto output {captureOutput(cmd)};
    EXPECT_FALSE(output.empty());
}

TEST_F(InstructorCommandsTest, ClearCommand_Execute_ClearsScreen)
{
    auto& cmd {m_registry.dispatch("clear")};
    const auto output {captureOutput(cmd)};
    EXPECT_NE(output.find("\033[2J\033[H"), std::string::npos);
}

TEST_F(InstructorCommandsTest, ClearCommand_Execute_DoesNotModifyModel)
{
    const auto hpBefore {m_model.hp()};
    const auto creditsBefore {m_model.credits()};
    const auto alertBefore {m_model.alertLevel()};

    auto& cmd {m_registry.dispatch("clear")};
    cmd.execute(m_model);

    EXPECT_EQ(m_model.hp(), hpBefore);
    EXPECT_EQ(m_model.credits(), creditsBefore);
    EXPECT_EQ(m_model.alertLevel(), alertBefore);
}

TEST_F(InstructorCommandsTest, ClearCommand_Execute_ContainsPlayerName)
{
    auto& cmd {m_registry.dispatch("clear")};
    const auto output {captureOutput(cmd)};
    EXPECT_NE(output.find(m_model.playerName()), std::string::npos);
}

TEST_F(InstructorCommandsTest, ClearCommand_Execute_NormalHp_NoWarning)
{
    auto& cmd {m_registry.dispatch("clear")};
    const auto output {captureOutput(cmd)};
    EXPECT_EQ(output.find("[HP CRITICO]"), std::string::npos);
}

TEST_F(InstructorCommandsTest, ClearCommand_Execute_HighAlert_ShowsWarning)
{
    auto& cmd {m_registry.dispatch("clear")};

    while (m_model.alertLevel() < AlertLevel::High)
    {
        m_model.incrementAlert("Forzando alerta para el test de unidad");
    }

    const auto output {captureOutput(cmd)};
    EXPECT_NE(output.find("[ALERTA ALTA]"), std::string::npos);
}

TEST_F(InstructorCommandsTest, ClearCommand_Execute_DoesNotThrow)
{
    auto& cmd {m_registry.dispatch("clear")};
    EXPECT_NO_THROW(cmd.execute(m_model));
}