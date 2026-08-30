//
// Created by oem on 26/3/26.
//

#include "locationCommandTest.hpp"
#include "model/locationCommand.hpp"

#include "model/gameModel.hpp"
#include <gtest/gtest.h>
#include <sstream>

using namespace CyberpunkCba;
///helper para capturar stdout

static std::string captureOutput (locationCommand& cmd, GameModel& model)
{
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    cmd.execute(model);
    std::cout.rdbuf(old);
    return oss.str();
}
/// Test 1 - Happy path: zona valida muestra info completa
TEST (LocationComand, Happypath)
{
    GameModel model ("Runner");
    ///currentZone() por defecto no debe ser ""unknown" ni vacio
    locationCommand cmd;
    std::string out = captureOutput(cmd, model);

    EXPECT_NE (out.find ("UBICACION DEL RUNNER"), std::string::npos);
    EXPECT_NE (out.find ("Zona"), std::string::npos);
    EXPECT_NE (out.find ("Peligro"), std::string::npos);

}

///Test 2 - Zona "unknown" muestra mensaje de senal perdida
TEST(LocationCommand, ZonaDesconocidaMuestraSenalPerdida)
{
    GameModel model ("Runner");
    locationCommand cmd;

}
///TEst 3 - Barra maxima con alertLevel :: maximum
TEST(LocationCommand, BarraPeligroMaxima)
{
    locationCommand cmd;
}

///TEst 4 - execute() no modifica GameModel
TEST(LocationCommand, GameModelModificado)
{
    GameModel model ("Runner");
    int creditsBefore = model.credits();
    int commandsBefore = model.commandCount();
    AlertLevel alertAntes = model.alertLevel();

    locationCommand cmd;
    cmd.execute(model);

    EXPECT_EQ(model.credits(), creditsBefore);
    EXPECT_EQ(model.commandCount(), commandsBefore);
    EXPECT_EQ(model.alertLevel(), alertAntes);
}
///TEst 5 - Salidas vacias no crashea y muestra mensaje
TEST(LocationCommand, SalidasVaciasNoCrashea)
{
    GameModel model ("Runner");
    locationCommand cmd;
    EXPECT_NO_THROW(cmd.execute(model));
    std::string out = captureOutput(cmd, model);
}
