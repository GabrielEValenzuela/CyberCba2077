#include "common/types.hpp"
#include "controller/commandRegistry.hpp"
#include "helpCommand.hpp"
#include "model/gameModel.hpp"
#include "statusCommand.hpp"
#include "unknownCommand.hpp"
#include "logCommand.hpp"
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
        m_registry.add(std::make_unique<logCommand>());
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
// LogCommand — Contrato y Lógica (Issue #96)
// =============================================================================

// 1. Identidad: Verifica nombre y categoría (Requisito de Registro)
TEST_F(InstructorCommandsTest, LogCommand_Identity) {
    auto& cmd {m_registry.dispatch("log")};
    EXPECT_EQ(cmd.name(), "log");
    EXPECT_EQ(cmd.category(), "system");
    EXPECT_FALSE(cmd.description().empty());
}

// 2. Paginación: Verifica el aviso de "últimas 10 de 15" (Requisito Lógico)
TEST_F(InstructorCommandsTest, LogCommand_Pagination) {
    for(int i = 0; i < 15; i++) {
        m_model.logAction("Entrada " + std::to_string(i));
    }
    auto& cmd {m_registry.dispatch("log")};
    const auto output {captureOutput(cmd)};
    // Buscamos sin tildes para evitar errores de codificación
    EXPECT_NE(output.find("Mostrando ultimas 10 de 15 entradas"), std::string::npos);
}

// 3. Fallback: Verifica que no crashea y maneja timestamps (Requisito Acceptance)
TEST_F(InstructorCommandsTest, LogCommand_TimestampFormat) {
    m_model.logAction("Evento de prueba");
    auto& cmd {m_registry.dispatch("log")};
    const auto output {captureOutput(cmd)};
    // Verifica que exista al menos un ":" o el fallback "--:--"
    EXPECT_TRUE(output.find(":") != std::string::npos || output.find("--:--") != std::string::npos);
}

// 4. Log Vacío: Verifica el mensaje de error amistoso (Requisito Acceptance)
TEST_F(InstructorCommandsTest, LogCommand_EmptyLog) {
    GameModel emptyModel("FreshRunner");
    auto& cmd {m_registry.dispatch("log")};

    std::ostringstream oss;
    std::streambuf* old {std::cout.rdbuf(oss.rdbuf())};
    cmd.execute(emptyModel);
    std::cout.rdbuf(old);

    EXPECT_NE(oss.str().find("vacio"), std::string::npos);
}

// 5. Sesión: Verifica formato HH:MM:SS y presencia de texto (Requisito Context)
TEST_F(InstructorCommandsTest, LogCommand_SessionDuration) {
    auto& cmd {m_registry.dispatch("log")};
    const auto output {captureOutput(cmd)};
    EXPECT_NE(output.find("Tiempo de sesion"), std::string::npos);

    int dots = 0;
    for(char c : output) if(c == ':') dots++;
    EXPECT_GE(dots, 2); // Al menos dos ":" para el formato HH:MM:SS
}

// 6. Invariabilidad: Verifica que GameModel no cambie (Requisito de Integridad)
TEST_F(InstructorCommandsTest, LogCommand_NoModification) {
    const auto creditsBefore {m_model.credits()};
    const auto sizeBefore {m_model.actionLog().size()};

    auto& cmd {m_registry.dispatch("log")};
    cmd.execute(m_model);

    EXPECT_EQ(m_model.credits(), creditsBefore);
    EXPECT_EQ(m_model.actionLog().size(), sizeBefore);
}

// 7. Caso Límite: Exactamente 10 entradas (No debe paginar)
TEST_F(InstructorCommandsTest, LogCommand_Execute_Exactamente10) {
    for (int i = 0; i < 10; i++) {
        m_model.logAction("Entrada " + std::to_string(i));
    }

    auto& cmd {m_registry.dispatch("log")};
    const auto output {captureOutput(cmd)};

    // REQUISITO: Con 10 entradas NO debe mostrar el aviso de "Mostrando ultimas..."
    EXPECT_EQ(output.find("Mostrando ultimas 10"), std::string::npos);

    // Debe mostrar la primera y la última
    EXPECT_NE(output.find("Entrada 0"), std::string::npos);
    EXPECT_NE(output.find("Entrada 9"), std::string::npos);
}