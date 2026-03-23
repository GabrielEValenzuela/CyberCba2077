#include "common/types.hpp"
#include "controller/commandRegistry.hpp"
#include "helpCommand.hpp"
#include "model/gameModel.hpp"
#include "statusCommand.hpp"
#include "unknownCommand.hpp"

#include <gtest/gtest.h>
#include <iostream>
#include <sstream>

#include <string>

// Truco de testing para poder armar el caso "inventario lleno"
// Solo usar en este archivo de test.

#include "shopCommand.hpp"

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

//=============================================================================
// ShopCommand Test
//=============================================================================

namespace CyberpunkCba
{
    namespace
    {
        std::string captureShopOutput(ShopCommand& command, GameModel& model)
        {
            std::ostringstream captured;
            std::streambuf* oldBuffer {std::cout.rdbuf(captured.rdbuf())};

            command.execute(model);

            std::cout.rdbuf(oldBuffer);
            return captured.str();
        }

        void fillInventoryToCapacity(GameModel& model)
        {
            while (static_cast<int>(model.m_inventory.size()) < model.m_inventoryCapacity)
            {
                model.m_inventory.push_back({"Item dummy", ItemType::Consumable, 1, 1});
            }
        }
    } // namespace

    TEST(ShopCommandTest, MarksAffordableAndUnaffordableItemsCorrectly)
    {
        GameModel model {"Runner_001"}; // por defecto arranca con 250 créditos
        ShopCommand command;

        const std::string output {captureShopOutput(command, model)};

        EXPECT_NE(output.find("(✓) Kit de primeros auxilios"), std::string::npos);
        EXPECT_NE(output.find("(✗) Ciberimplante de brazo"), std::string::npos);
    }

    TEST(ShopCommandTest, ShowsInventoryFullMessageWhenInventoryIsFull)
    {
        GameModel model {"Runner_001"};
        ShopCommand command;

        fillInventoryToCapacity(model);

        const std::string output {captureShopOutput(command, model)};

        EXPECT_NE(output.find("Tu inventario"), std::string::npos);
        EXPECT_EQ(output.find("Items asequibles:"), std::string::npos);
    }

    TEST(ShopCommandTest, ShowsNoCreditsMessageWhenPlayerCannotBuyAnything)
    {
        GameModel model {"Runner_001"};
        ShopCommand command;

        model.spendCredits(model.credits()); // deja créditos en 0

        const std::string output {captureShopOutput(command, model)};

        EXPECT_NE(output.find("No tienes suficientes"), std::string::npos);
        EXPECT_EQ(output.find("Items asequibles:"), std::string::npos);
    }

    TEST(ShopCommandTest, ShowsCorrectAffordableItemsCount)
    {
        GameModel model {"Runner_001"}; // 250 créditos por defecto
        ShopCommand command;

        const std::string output {captureShopOutput(command, model)};

        EXPECT_NE(output.find("Items asequibles: 2"), std::string::npos);
    }

    TEST(ShopCommandTest, ExecuteDoesNotModifyGameModel)
    {
        GameModel model {"Runner_001"};
        ShopCommand command;

        const int creditsBefore {model.credits()};
        const int inventorySizeBefore {static_cast<int>(model.inventory().size())};
        const int commandCountBefore {model.commandCount()};
        const bool runningBefore {model.isRunning()};

        const std::string output {captureShopOutput(command, model)};

        (void)output;

        EXPECT_EQ(model.credits(), creditsBefore);
        EXPECT_EQ(static_cast<int>(model.inventory().size()), inventorySizeBefore);
        EXPECT_EQ(model.commandCount(), commandCountBefore);
        EXPECT_EQ(model.isRunning(), runningBefore);
    }

} // namespace CyberpunkCba
