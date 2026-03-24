#include "clearCommand.hpp"
#include "controller/commandRegistry.hpp"
#include "model/gameModel.hpp"

#include <gtest/gtest.h>
#include <iostream>
#include <sstream>

using namespace CyberpunkCba;

// =============================================================================
// Fixture específico para ClearCommand
// =============================================================================

class ClearCommandTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Registramos exclusivamente el comando que vamos a testear
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
// Tests de contrato y comportamiento
// =============================================================================

TEST_F(ClearCommandTest, Name)
{
    auto& cmd {m_registry.dispatch("clear")};
    EXPECT_EQ(cmd.name(), "clear");
}

TEST_F(ClearCommandTest, Category)
{
    auto& cmd {m_registry.dispatch("clear")};
    EXPECT_EQ(cmd.category(), "sistema");
}

TEST_F(ClearCommandTest, DescriptionNotEmpty)
{
    auto& cmd {m_registry.dispatch("clear")};
    EXPECT_FALSE(cmd.description().empty());
}

TEST_F(ClearCommandTest, Execute_ProducesOutput)
{
    auto& cmd {m_registry.dispatch("clear")};
    const auto output {captureOutput(cmd)};

    // Verifica que el output no esté vacío
    EXPECT_FALSE(output.empty());
}

TEST_F(ClearCommandTest, Execute_ClearsScreen)
{
    auto& cmd {m_registry.dispatch("clear")};
    const auto output {captureOutput(cmd)};

    // Verifica que contenga la secuencia ANSI para limpiar la pantalla
    EXPECT_NE(output.find("\033[2J\033[H"), std::string::npos);
}

TEST_F(ClearCommandTest, Execute_DoesNotModifyModel)
{
    const auto hpBefore {m_model.hp()};
    const auto creditsBefore {m_model.credits()};
    const auto alertBefore {m_model.alertLevel()};

    auto& cmd {m_registry.dispatch("clear")};
    cmd.execute(m_model);

    // Validamos que el modelo quede intacto tras el clear
    EXPECT_EQ(m_model.hp(), hpBefore);
    EXPECT_EQ(m_model.credits(), creditsBefore);
    EXPECT_EQ(m_model.alertLevel(), alertBefore);
}

// 1. header contiene nombre del runner
TEST_F(ClearCommandTest, Execute_ContainsPlayerName)
{
    auto& cmd {m_registry.dispatch("clear")};
    const auto output {captureOutput(cmd)};

    EXPECT_NE(output.find(m_model.playerName()), std::string::npos);
}

// 2. HP normal sin advertencia
TEST_F(ClearCommandTest, Execute_NormalHp_NoWarning)
{
    auto& cmd {m_registry.dispatch("clear")};
    const auto output {captureOutput(cmd)};

    // Al instanciar GameModel, el HP está al máximo. No debe decir HP CRITICO.
    EXPECT_EQ(output.find("[HP CRITICO]"), std::string::npos);
}

// 3. alerta alta muestra advertencia
TEST_F(ClearCommandTest, Execute_HighAlert_ShowsWarning)
{
    auto& cmd {m_registry.dispatch("clear")};

    // Usamos la API pública de GameModel para subir la alerta hasta High
    while (m_model.alertLevel() < AlertLevel::High)
    {
        m_model.incrementAlert("Forzando alerta para el test de unidad");
    }

    const auto output {captureOutput(cmd)};
    EXPECT_NE(output.find("[ALERTA ALTA]"), std::string::npos);
}

// 4. execute() no lanza excepciones
TEST_F(ClearCommandTest, Execute_DoesNotThrow)
{
    auto& cmd {m_registry.dispatch("clear")};

    // GTest provee esta macro para asegurar que el método no lance excepciones (crashee)
    EXPECT_NO_THROW(cmd.execute(m_model));
}
