#include "model/GameModel.hpp"
#include <gtest/gtest.h>

TEST(GameModel, CreditsStartAtZero)
{
    cyberpunk::GameModel model;
    EXPECT_EQ(model.credits(), 0);
}

TEST(GameModel, AddCreditsWorks)
{
    cyberpunk::GameModel model;
    model.addCredits(10);
    model.addCredits(15);
    EXPECT_EQ(model.credits(), 25);
}
