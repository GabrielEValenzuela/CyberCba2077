#include "common/types.hpp"
#include "model/gameModel.hpp"
#include "controller/command.hpp"
#include "InventoryCommand.h"

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
    Command&         m_cmdIface {m_cmd};  // acceso por interfaz base (métodos son private)
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
    // quantity=1 → debe aparecer "x1"
    const std::string output {captureOutput(m_cmdIface)};
    EXPECT_NE(output.find("x1"), std::string::npos);
}

TEST_F(InventoryCommandTest, OutputDoesNotShowVacioMessage)
{
    // Hay un ítem → no debe mostrar [VACIO]
    const std::string output {captureOutput(m_cmdIface)};
    EXPECT_EQ(output.find("[VACIO]"), std::string::npos);
}

TEST_F(InventoryCommandTest, OutputDoesNotShowCuidadoWarning)
{
    // 1 ítem de 10 → no debe mostrar [CUIDADO]
    const std::string output {captureOutput(m_cmdIface)};
    EXPECT_EQ(output.find("[CUIDADO]"), std::string::npos);
}

// =============================================================================
// Valor total — price=80, quantity=1 → 80.0 créditos
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

// =============================================================================
// Barra de carga — 1 ítem sobre 10, ancho 15
// renderBar(1, 10, 15) → 1 '#' y 14 '.'
// =============================================================================

TEST_F(InventoryCommandTest, LoadBarLabelIsPresent)
{
    const std::string output {captureOutput(m_cmdIface)};
    EXPECT_NE(output.find("Carga actual:"), std::string::npos);
}

TEST_F(InventoryCommandTest, LoadBarHasBrackets)
{
    const std::string output {captureOutput(m_cmdIface)};
    const auto barStart {output.find('[')};
    const auto barEnd   {output.find(']', barStart)};

    ASSERT_NE(barStart, std::string::npos);
    EXPECT_NE(barEnd,   std::string::npos);
}

TEST_F(InventoryCommandTest, LoadBarWidthIsFifteen)
{
    const std::string output {captureOutput(m_cmdIface)};
    const auto barStart {output.find('[')};
    const auto barEnd   {output.find(']', barStart)};

    ASSERT_NE(barStart, std::string::npos);
    ASSERT_NE(barEnd,   std::string::npos);

    const std::size_t barWidth {barEnd - barStart - 1};
    EXPECT_EQ(barWidth, 15u);
}

TEST_F(InventoryCommandTest, LoadBarContainsBothHashAndDot)
{
    // 1/10 → al menos un '#' y al menos un '.'
    const std::string output {captureOutput(m_cmdIface)};
    const auto barStart {output.find('[')};
    const auto barEnd   {output.find(']', barStart)};

    ASSERT_NE(barStart, std::string::npos);
    ASSERT_NE(barEnd,   std::string::npos);

    const std::string barContent {output.substr(barStart + 1, barEnd - barStart - 1)};
    EXPECT_NE(barContent.find('#'), std::string::npos);
    EXPECT_NE(barContent.find('.'), std::string::npos);
}

// =============================================================================
// Estructura general de la salida
// =============================================================================

TEST_F(InventoryCommandTest, OutputIsNotEmpty)
{
    const std::string output {captureOutput(m_cmdIface)};
    EXPECT_FALSE(output.empty());
}

TEST_F(InventoryCommandTest, OutputContainsAllThreeSections)
{
    const std::string output {captureOutput(m_cmdIface)};
    EXPECT_NE(output.find("Inventario de"), std::string::npos);
    EXPECT_NE(output.find("Valor total:"),  std::string::npos);
    EXPECT_NE(output.find("Carga actual:"), std::string::npos);
}