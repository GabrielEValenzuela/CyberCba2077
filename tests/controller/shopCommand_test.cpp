/**#include <gtest/gtest.h>

#include <iostream>
#include <sstream>
#include <string>

// Truco de testing para poder armar el caso "inventario lleno"
// Solo usar en este archivo de test.
#define private public
#include "model/gameModel.hpp"
#undef private

#include "shopCommand.hpp"
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
 */