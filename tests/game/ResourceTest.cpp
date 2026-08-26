#include "cybercba/game/Resources.hpp"

#include <gtest/gtest.h>
#include <iostream>

namespace cybercba::game
{
namespace
{

class ResourceTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        std::cout << "esto es una prueba de setup" << std::endl;
    }

    void TearDown() override
    {
        std::cout << "esto es una prueba de teardown" << std::endl;
    }
};

TEST_F(ResourceTest, HAPPYPATH)
{
    const int value = 42;
    EXPECT_EQ(value, 42);
}

} // namespace
} // namespace cybercba::game
