#include "controller/commandCredits.hpp"
#include "model/gameModel.hpp"
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace CyberpunkCba;

class CommandCreditsTest : public ::testing::Test
{
protected:
    GameModel model {"RunnerTest"};
    std::stringstream buffer;
    std::streambuf* oldCout;

    void SetUp() override
    {
        oldCout = std::cout.rdbuf(buffer.rdbuf());
    }

    void TearDown() override
    {
        std::cout.rdbuf(oldCout);
    }
};

TEST_F(CommandCreditsTest, VectorVacioNoCrashea)
{
    std::vector<TeamMember> emptyTeam;
    CommandCredits cmd(emptyTeam, "TeamTest");
    Command& baseCmd = cmd;
    ASSERT_NO_THROW(baseCmd.execute(model));
    std::string output = buffer.str();
    EXPECT_TRUE(output.find("Error") != std::string::npos || output.find("No hay miembros") != std::string::npos);
}

TEST_F(CommandCreditsTest, EquipoIncompletoMuestraAdvertencia)
{
    std::vector<TeamMember> team = {{"Juan", "Dev"}};
    CommandCredits cmd(team, "TeamTest");
    Command& baseCmd = cmd;
    baseCmd.execute(model);
    EXPECT_TRUE(buffer.str().find("ADVERTENCIA") != std::string::npos);
}

TEST_F(CommandCreditsTest, HappyPathEquipoCompleto)
{
    std::vector<TeamMember> team = {{"A", "Dev"}, {"B", "QA"}, {"C", "PM"}, {"D", "UI"}};
    CommandCredits cmd(team, "DreamTeam");
    Command& baseCmd = cmd;
    baseCmd.execute(model);
    std::string output = buffer.str();
    EXPECT_TRUE(output.find("=== EQUIPO DE DESARROLLO: DreamTeam ===") != std::string::npos);
    EXPECT_TRUE(output.find("ADVERTENCIA") == std::string::npos);
}

TEST_F(CommandCreditsTest, PaddingCalculadoCorrectamente)
{
    std::vector<TeamMember> team = {{"Ana", "Dev"}, {"Carlos", "Senior Software Engineer"}};
    CommandCredits cmd(team, "TeamTest");
    Command& baseCmd = cmd;
    baseCmd.execute(model);
    std::string output = buffer.str();
    std::string lineaEsperada = "Dev";
    lineaEsperada.append(21, ' ');
    lineaEsperada += " | Ana";
    EXPECT_TRUE(output.find(lineaEsperada) != std::string::npos);
}

TEST_F(CommandCreditsTest, GameModelNoEsModificado)
{
    int hpAntes = model.hp();
    int creditsAntes = model.credits();
    int alertAntes = static_cast<int>(model.alertLevel());
    std::vector<TeamMember> team = {{"A", "B"}, {"C", "D"}, {"E", "F"}, {"G", "H"}};
    CommandCredits cmd(team, "TeamTest");
    Command& baseCmd = cmd;
    baseCmd.execute(model);
    EXPECT_EQ(model.hp(), hpAntes);
    EXPECT_EQ(model.credits(), creditsAntes);
    EXPECT_EQ(static_cast<int>(model.alertLevel()), alertAntes);
}