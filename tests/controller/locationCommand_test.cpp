#include <gtest/gtest.h>
#include <sstream>
#include "controller/locationCommand.hpp"
#include "model/gameModel.hpp"

using namespace CyberpunkCba;

// Helper para capturar stdout
static std::string captureOutput(locationCommand& cmd, GameModel& model)
{
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    cmd.execute(model);
    std::cout.rdbuf(old);
    return oss.str();
}

// Test 1 — Happy path: zona válida muestra info completa
TEST(locationCommand, HappyPath)
{
    GameModel model("Runner");
    // currentZone() por defecto no debe ser "unknown" ni vacío
    locationCommand cmd;
    std::string out = captureOutput(cmd, model);

    EXPECT_NE(out.find("UBICACION DEL RUNNER"), std::string::npos);
    EXPECT_NE(out.find("Zona"), std::string::npos);
    EXPECT_NE(out.find("Peligro"), std::string::npos);
}

// Test 2 — Zona "unknown" muestra mensaje de señal perdida
TEST(locationCommand, ZonaDesconocidaMuestraSenalPerdida)
{
    // Necesitás poder setear currentZone a "unknown"
    // Si GameModel tiene setter o lo inicializa así en algún estado,
    // ajustá acá. Si no, consultá con tu equipo cómo mockearlo.
    GameModel model("Runner");
    // model.setCurrentZone("unknown");  ← cuando tengas el setter
    locationCommand cmd;
    // std::string out = captureOutput(cmd, model);
    // EXPECT_NE(out.find("SEÑAL PERDIDA"), std::string::npos);
}

// Test 3 — Barra máxima con AlertLevel::Maximum
TEST(locationCommand, BarraPeligroMaxima)
{
    locationCommand cmd;
    // Accedemos al método privado a través de una subclase de test
    // o lo testeamos indirectamente via execute() con alertLevel Maximum
    // Salida esperada: "[##########] ..."
    // Ajustá según cómo exponés buildDangerBar en tu proyecto
}

// Test 4 — execute() no modifica GameModel
TEST(LocationCommand, GameModelNoModificado)
{
    GameModel model("Runner");
    int creditsBefore  = model.credits();
    int commandsBefore = model.commandCount();
    AlertLevel alertAntes = model.alertLevel();

    locationCommand cmd;
    cmd.execute(model);

    EXPECT_EQ(model.credits(),      creditsBefore);
    EXPECT_EQ(model.commandCount(), commandsBefore);
    EXPECT_EQ(model.alertLevel(), alertAntes);
}

// Test 5 — Salidas vacías no crashea y muestra mensaje
TEST(LocationCommand, SalidasVaciasNoCrashea)
{
    GameModel model("Runner");
    locationCommand cmd;
    // Si adjacentZones() arranca vacío, esto alcanza
    EXPECT_NO_THROW(cmd.execute(model));
    std::string out = captureOutput(cmd, model);
    // Si la zona es válida:
    // EXPECT_NE(out.find("Sin salidas conocidas"), std::string::npos);
}