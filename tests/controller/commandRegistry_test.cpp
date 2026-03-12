#include "controller/command.hpp"
#include "controller/commandRegistry.hpp"
#include "model/gameModel.hpp"
#include "unknownCommand.hpp"

#include <gtest/gtest.h>
#include <memory>
#include <string>

using namespace CyberpunkCba;

// =============================================================================
// Comando de prueba mínimo
// =============================================================================

class FakeCommand final : public Command
{
public:
    explicit FakeCommand(std::string cmdName, std::string cat = "test")
        : m_name {std::move(cmdName)}
        , m_cat {std::move(cat)}
    {
    }

    bool wasExecuted() const noexcept
    {
        return m_executed;
    }

private:
    void execute(GameModel& /*model*/) override
    {
        m_executed = true;
    }
    std::string name() const override
    {
        return m_name;
    }
    std::string description() const override
    {
        return "fake";
    }
    std::string category() const override
    {
        return m_cat;
    }

    std::string m_name;
    std::string m_cat;
    bool m_executed {false};
};

// =============================================================================
// Fixture
// =============================================================================

class CommandRegistryTest : public ::testing::Test
{
protected:
    CommandRegistry m_registry;
    GameModel m_model {"TestRunner"};
};

// =============================================================================
// add / contains
// =============================================================================

TEST_F(CommandRegistryTest, Add_ContainsAfterAdd)
{
    m_registry.add(std::make_unique<FakeCommand>("test"));
    EXPECT_TRUE(m_registry.contains("test"));
}

TEST_F(CommandRegistryTest, Add_SizeIncrements)
{
    EXPECT_EQ(m_registry.size(), 0u);
    m_registry.add(std::make_unique<FakeCommand>("a"));
    m_registry.add(std::make_unique<FakeCommand>("b"));
    EXPECT_EQ(m_registry.size(), 2u);
}

TEST_F(CommandRegistryTest, Add_ReplaceExisting_SizeUnchanged)
{
    m_registry.add(std::make_unique<FakeCommand>("dup"));
    m_registry.add(std::make_unique<FakeCommand>("dup")); // reemplaza
    EXPECT_EQ(m_registry.size(), 1u);
}

TEST_F(CommandRegistryTest, Contains_ReturnsFalseForUnknown)
{
    EXPECT_FALSE(m_registry.contains("nonexistent"));
}

// =============================================================================
// dispatch
// =============================================================================

TEST_F(CommandRegistryTest, Dispatch_KnownCommand_ReturnsIt)
{
    m_registry.add(std::make_unique<FakeCommand>("ping"));
    auto& cmd {m_registry.dispatch("ping")};
    EXPECT_EQ(cmd.name(), "ping");
}

TEST_F(CommandRegistryTest, Dispatch_ExecutesCorrectCommand)
{
    auto spFake {std::make_unique<FakeCommand>("run")};
    auto* pFake {spFake.get()};
    m_registry.add(std::move(spFake));

    m_registry.dispatch("run").execute(m_model);
    EXPECT_TRUE(pFake->wasExecuted());
}

TEST_F(CommandRegistryTest, Dispatch_UnknownInput_ReturnsUnknownCommand)
{
    CommandRegistry reg;
    reg.add(std::make_unique<UnknownCommand>(""));
    reg.add(std::make_unique<FakeCommand>("real"));

    auto& cmd {reg.dispatch("esto_no_existe")};
    EXPECT_EQ(cmd.name(), "__unknown");
}

// =============================================================================
// commands() — orden de registro
// =============================================================================

TEST_F(CommandRegistryTest, Commands_PreservesRegistrationOrder)
{
    m_registry.add(std::make_unique<FakeCommand>("primero"));
    m_registry.add(std::make_unique<FakeCommand>("segundo"));
    m_registry.add(std::make_unique<FakeCommand>("tercero"));

    const auto& list {m_registry.commands()};
    ASSERT_EQ(list.size(), 3u);
    EXPECT_EQ(list[0]->name(), "primero");
    EXPECT_EQ(list[1]->name(), "segundo");
    EXPECT_EQ(list[2]->name(), "tercero");
}

TEST_F(CommandRegistryTest, Commands_NoNullptrs)
{
    m_registry.add(std::make_unique<FakeCommand>("x"));
    for (const auto* pCmd : m_registry.commands())
    {
        EXPECT_NE(pCmd, nullptr);
    }
}

// =============================================================================
// empty / size
// =============================================================================

TEST_F(CommandRegistryTest, Empty_TrueWhenEmpty)
{
    EXPECT_TRUE(m_registry.empty());
}

TEST_F(CommandRegistryTest, Empty_FalseAfterAdd)
{
    m_registry.add(std::make_unique<FakeCommand>("z"));
    EXPECT_FALSE(m_registry.empty());
}

// =============================================================================
// buildRegistry — smoke test
// =============================================================================

TEST(BuildRegistryTest, BuildRegistry_AllStubsRegistered)
{
    auto registry {buildRegistry()};

    EXPECT_TRUE(registry.contains("help"));
    EXPECT_TRUE(registry.contains("status"));
    // ============================================================
    // ZONA DE EQUIPOS - Agregar EXPECT_TRUE(registry.contains("TuComando")) por cada comando registrado
    // ============================================================

    // ============================================================
    // FIN ZONA DE EQUIPOS
    // ============================================================
    EXPECT_FALSE(registry.empty());
}
