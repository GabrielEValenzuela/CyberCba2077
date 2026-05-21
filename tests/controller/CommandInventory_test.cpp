#include "InventoryCommand.hpp"
#include "common/types.hpp"
#include "controller/command.hpp"
#include "model/gameModel.hpp"

#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <string>

using namespace CyberpunkCba;

// =============================================================================
// Estado inicial conocido del GameModel
//
//   Ítem inicial : "Deck de hackeo básico"  price=80  quantity=1
//   Valor total  : 80.0 créditos
//   Capacidad    : 10
//   Ítems actuales: 1  →  NO vacío, NO lleno
// =============================================================================

class InventoryCommandTest : public ::testing::Test
{
protected:
    /// @brief Captura stdout durante la ejecución de un comando.
    std::string captureOutput(Command& cmd)
    {
        std::ostringstream oss;
        std::streambuf* old {std::cout.rdbuf(oss.rdbuf())};
        cmd.execute(m_model);
        std::cout.rdbuf(old);
        return oss.str();
    }

    GameModel        m_model {"Ghost_47"};
    InventoryCommand m_cmd;
    Command&         m_cmdIface {m_cmd};
};

// =============================================================================
// Metadatos del comando
// =============================================================================

TEST_F(InventoryCommandTest, NameReturnsInventario)
{
    EXPECT_EQ(m_cmdIface.name(), "inventario");
}

TEST_F(InventoryCommandTest, DescriptionIsNotEmpty)
{
    EXPECT_FALSE(m_cmdIface.description().empty());
}

TEST_F(InventoryCommandTest, CategoryReturnsRunner)
{
    EXPECT_EQ(m_cmdIface.category(), "runner");
}

// =============================================================================
// Estado inicial — el modelo arranca con 1 ítem, no vacío, no lleno
// =============================================================================

TEST_F(InventoryCommandTest, InitialStateHasOneItem)
{
    ASSERT_EQ(m_model.inventory().size(), 1u);
}

TEST_F(InventoryCommandTest, InitialStateIsNotEmpty)
{
    EXPECT_FALSE(m_model.inventory().empty());
}

TEST_F(InventoryCommandTest, InitialStateIsNotFull)
{
    EXPECT_FALSE(m_model.isInventoryFull());
}

TEST_F(InventoryCommandTest, CapacityIsTen)
{
    EXPECT_EQ(m_model.inventoryCapacity(), 10);
}

// =============================================================================
// Salida con inventario con ítems (estado inicial)
// =============================================================================

TEST_F(InventoryCommandTest, OutputShowsPlayerNameInHeader)
{
    const std::string output {captureOutput(m_cmdIface)};
    EXPECT_NE(output.find("--- Inventario de Ghost_47 ---"), std::string::npos);
}

TEST_F(InventoryCommandTest, OutputShowsInitialItemName)
{
    const std::string output {captureOutput(m_cmdIface)};
    EXPECT_NE(output.find("Deck de hackeo básico"), std::string::npos);
}

TEST_F(InventoryCommandTest, OutputShowsInitialItemQuantity)
{
    const std::string output {captureOutput(m_cmdIface)};
    EXPECT_NE(output.find("x1"), std::string::npos);
}

TEST_F(InventoryCommandTest, OutputDoesNotShowVacioMessage)
{
    const std::string output {captureOutput(m_cmdIface)};
    EXPECT_EQ(output.find("[VACIO]"), std::string::npos);
}

// =============================================================================
// LEGACY: Tests de la versión anterior con renderBar.
// Comentados porque la versión actual no implementa barra de carga.
// =============================================================================

// TEST_F(InventoryCommandTest, OutputDoesNotShowCuidadoWarning) { ... }
// TEST_F(InventoryCommandTest, LoadBarLabelIsPresent) { ... }
// TEST_F(InventoryCommandTest, LoadBarHasBrackets) { ... }
// TEST_F(InventoryCommandTest, LoadBarWidthIsFifteen) { ... }
// TEST_F(InventoryCommandTest, LoadBarContainsBothHashAndDot) { ... }
// TEST_F(InventoryCommandTest, OutputContainsAllThreeSections) { ... }

// =============================================================================
// Valor total
// =============================================================================

TEST_F(InventoryCommandTest, TotalValueMatchesInitialItem)
{
    const std::string output {captureOutput(m_cmdIface)};
    EXPECT_NE(output.find("80"), std::string::npos);
}

TEST_F(InventoryCommandTest, TotalValueLabelIsPresent)
{
    const std::string output {captureOutput(m_cmdIface)};
    EXPECT_NE(output.find("Valor total:"), std::string::npos);
}

TEST_F(InventoryCommandTest, TotalValueSufixIsCreditos)
{
    const std::string output {captureOutput(m_cmdIface)};
    EXPECT_NE(output.find("creditos"), std::string::npos);
}

TEST_F(InventoryCommandTest, OutputIsNotEmpty)
{
    const std::string output {captureOutput(m_cmdIface)};
    EXPECT_FALSE(output.empty());
}

TEST_F(InventoryCommandTest, OutputShowsValorTotal)
{
    const std::string output {captureOutput(m_cmdIface)};
    EXPECT_NE(output.find("Valor total:"), std::string::npos);
}

TEST_F(InventoryCommandTest, EmptyFileShowsVacioMessage)
{
    GameModel        emptyModel {"Runner_Test"};
    InventoryCommand cmd;
    Command&         iface {cmd};

    std::ostringstream oss;
    std::streambuf* old {std::cout.rdbuf(oss.rdbuf())};
    iface.execute(emptyModel);
    std::cout.rdbuf(old);

    const std::string output {oss.str()};
    EXPECT_NE(output.find("[VACIO]"), std::string::npos);
}

// =============================================================================
// SET A: inventoryValue — acumulación recursiva
// =============================================================================

TEST(InventoryValueTest, EmptyInventoryReturnsZero)
{
    std::vector<Item> inv;
    EXPECT_EQ(InventoryCommand::inventoryValue(inv, 0), 0);
}

TEST(InventoryValueTest, SingleItemReturnsItsPrice)
{
    std::vector<Item> inv;
    inv.push_back({"Pistola", ItemType::Weapon, 100, 1});
    EXPECT_EQ(InventoryCommand::inventoryValue(inv, 0), 100);
}

TEST(InventoryValueTest, MultipleItemsSumsCorrectly)
{
    std::vector<Item> inv;
    inv.push_back({"Pistola", ItemType::Weapon,    100, 2});
    inv.push_back({"Medkit",  ItemType::Consumable,  50, 3});
    inv.push_back({"Chip",    ItemType::Tech,        200, 1});
    // 100*2 + 50*3 + 200*1 = 200 + 150 + 200 = 550
    EXPECT_EQ(InventoryCommand::inventoryValue(inv, 0), 550);
}

TEST(InventoryValueTest, NegativePriceItemIsIgnored)
{
    std::vector<Item> inv;
    inv.push_back({"ItemMalo",  ItemType::Value, -10, 1});
    inv.push_back({"ItemBueno", ItemType::Value,  50, 1});
    EXPECT_EQ(InventoryCommand::inventoryValue(inv, 0), 50);
}

TEST(InventoryValueTest, NegativeQuantityItemIsIgnored)
{
    std::vector<Item> inv;
    inv.push_back({"ItemMalo",  ItemType::Value, 50, -1});
    inv.push_back({"ItemBueno", ItemType::Value, 30,  2});
    EXPECT_EQ(InventoryCommand::inventoryValue(inv, 0), 60);
}