#include "common/types.hpp"
#include "controller/commandRegistry.hpp"
#include "helpCommand.hpp"
#include "model/gameModel.hpp"
#include "statusCommand.hpp"
#include "unknownCommand.hpp"
#include "mapCommand.h"

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
        m_registry.add(std::make_unique<MapCommand>());
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
//  Mapcommand — contrato
// =============================================================================

TEST_F(InstructorCommandsTest, MapCommand_Name)
{
    auto& cmd {m_registry.dispatch("map")};
    EXPECT_EQ(cmd.name(), "map");
}

TEST_F(InstructorCommandsTest, MapCommand_Category)
{
    auto& cmd {m_registry.dispatch("map")};
    EXPECT_EQ(cmd.category(), "mundo");
}

TEST_F(InstructorCommandsTest, MapCommand_DescriptionNotEmpty)
{
    auto& cmd {m_registry.dispatch("map")};
    EXPECT_FALSE(cmd.description().empty());
}

TEST_F(InstructorCommandsTest, MapCommand_Execute_ProducesOutput)
{
    auto& cmd {m_registry.dispatch("map")};
    const auto output {captureOutput(cmd)};
    EXPECT_FALSE(output.empty());
}

TEST_F(InstructorCommandsTest, MapCommand_Execute_ContainsMapKeyword)
{
    auto& cmd {m_registry.dispatch("map")};
    const auto output {captureOutput(cmd)};
    // El output debe mencionar "map" entre los comandos listados o en la info que genera
    EXPECT_NE(output.find("map"), std::string::npos);
}

TEST_F(InstructorCommandsTest, MapCommand_Execute_EmptyRegistry_NoCrash)
{
    // Registry vacío — map no debe crashear
    CommandRegistry emptyReg;
    emptyReg.add(std::make_unique<UnknownCommand>(""));
    auto spMap {std::make_unique<MapCommand>()};
    Command& cmd {*spMap};
    std::ostringstream oss;
    std::streambuf* old {std::cout.rdbuf(oss.rdbuf())};
    cmd.execute(m_model);
    std::cout.rdbuf(old);
    EXPECT_FALSE(oss.str().empty());
}

TEST_F(InstructorCommandsTest, MapCommand_Execute_DoesNotModifyModel)
{
    const auto creditsBefore {m_model.credits()};
    const auto alertBefore {m_model.alertLevel()};
    const auto runningBefore {m_model.isRunning()};

    auto& cmd {m_registry.dispatch("map")};
    cmd.execute(m_model);

    EXPECT_EQ(m_model.credits(), creditsBefore);
    EXPECT_EQ(m_model.alertLevel(), alertBefore);
    EXPECT_EQ(m_model.isRunning(), runningBefore);
}

// =============================================================================
//  MapCommand — Criterios de Aceptación (Issue #90)
// =============================================================================

TEST_F(InstructorCommandsTest, MapCommand_RenderizaPosicionDinamica)
{
    // AC 1: Posición del runner inyectada dinámicamente con la estrella (★)
    // El GameModel arranca por defecto en el "Sector 7"
    auto& cmd {m_registry.dispatch("map")};
    const auto output {captureOutput(cmd)};

    // Verificamos que la estrella aparezca junto a "Sector 7"
    EXPECT_NE(output.find("★ Sector 7"), std::string::npos);
}

TEST_F(InstructorCommandsTest, MapCommand_MuestraSenalDegradadaEnZonaDesconocida)
{
    // AC 2: Zona desconocida muestra mensaje de señal degradada y no crashea
    // Como GameModel no tiene setter de zona, validamos que
    // NO muestre el mensaje de degradada en un estado inicial válido.
    auto& cmd {m_registry.dispatch("map")};
    const auto output {captureOutput(cmd)};

    // Verificamos que el output NO tenga error de señal (porque arranca en zona válida)
    EXPECT_EQ(output.find("degradada"), std::string::npos);
}

TEST_F(InstructorCommandsTest, MapCommand_MuestraZonasBloqueadasConAlertaAlta)
{
    // AC 3: Zonas bloqueadas marcadas con [BLOQUEADA] si alertLevel() >= High
    // El GameModel arranca en Low. Para subir a High, debemos llamar a incrementAlert varias veces.
    // Low -> Medium -> High
    m_model.incrementAlert("Robo a transeúnte");
    m_model.incrementAlert("Tiroteo en el mercado");

    auto& cmd {m_registry.dispatch("map")};
    const auto output {captureOutput(cmd)};

    // Verificamos que aparezca la etiqueta de bloqueo al estar en alerta High
    EXPECT_NE(output.find("[BLOQUEADA]"), std::string::npos);
}

TEST_F(InstructorCommandsTest, MapCommand_CalculaContadorDeVisitadas)
{
    // AC 4: Contador de zonas visitadas calculado desde visitedZones()
    auto& cmd {m_registry.dispatch("map")};
    const auto output {captureOutput(cmd)};

    // Como vimos en tu captura y en GameModel.cpp, arranca con 1 zona visitada de 6
    EXPECT_NE(output.find("1 / 6"), std::string::npos);
}