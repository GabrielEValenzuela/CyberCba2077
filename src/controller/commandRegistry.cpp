#include "controller/commandRegistry.hpp"

// Comandos del instructor — completos
#include "helpCommand.hpp"
#include "statusCommand.hpp"
#include "unknownCommand.hpp"

// ============================================================
// ZONA DE EQUIPOS — agregar un #include por equipo
// ============================================================
#include "clearCommand.hpp"
// ============================================================
// FIN ZONA DE EQUIPOS
// ============================================================

#include "clearCommand.hpp"

#include <cassert>

namespace CyberpunkCba
{

    // =============================================================================
    // CommandRegistry
    // =============================================================================

    void CommandRegistry::add(std::unique_ptr<Command> spCommand)
    {
        assert(spCommand != nullptr);
        assert(!spCommand->name().empty());

        const auto commandName {spCommand->name()};

        // Registrar puntero observador en m_pUnknownCommand si corresponde
        if (commandName == "__unknown")
        {
            m_pUnknownCommand = spCommand.get();
        }

        // Si ya existe un comando con ese nombre, reemplazar también en la lista
        const auto it {m_commandMap.find(commandName)};
        if (it != m_commandMap.end())
        {
            // Actualizar puntero en m_commandList
            for (auto*& pCmd : m_commandList)
            {
                if (pCmd->name() == commandName)
                {
                    it->second = std::move(spCommand);
                    pCmd = it->second.get();
                    return;
                }
            }
        }

        // Nuevo comando
        m_commandMap.emplace(commandName, std::move(spCommand));
        m_commandList.emplace_back(m_commandMap.at(commandName).get());
    }

    Command& CommandRegistry::dispatch(const std::string& input)
    {
        const auto it {m_commandMap.find(input)};
        if (it != m_commandMap.end())
        {
            return *it->second;
        }

        // Fallback: UnknownCommand con el input actual
        if (m_pUnknownCommand != nullptr)
        {
            static_cast<UnknownCommand*>(m_pUnknownCommand)->setInput(input);
            return *m_pUnknownCommand;
        }
        // Sin UnknownCommand — solo posible en tests unitarios del registry
        assert(!m_commandMap.empty());
        return *m_commandMap.begin()->second;
    }

    bool CommandRegistry::contains(const std::string& commandName) const
    {
        return m_commandMap.count(commandName) > 0;
    }

    const std::vector<const Command*>& CommandRegistry::commands() const noexcept
    {
        return m_commandList;
    }

    std::size_t CommandRegistry::size() const noexcept
    {
        return m_commandMap.size();
    }
    bool CommandRegistry::empty() const noexcept
    {
        return m_commandMap.empty();
    }

    // =============================================================================
    // buildRegistry — factory
    // =============================================================================

    CommandRegistry buildRegistry()
    {
        CommandRegistry registry;

        // Comandos del instructor — no tocar
        registry.add(std::make_unique<UnknownCommand>(""));

        auto spHelp {std::make_unique<HelpCommand>(registry)};
        registry.add(std::move(spHelp));
        registry.add(std::make_unique<StatusCommand>());

        // ============================================================
        // ZONA DE EQUIPOS — agregar una línea por equipo
        // Formato: registry.add(std::make_unique<TuComandoCommand>());
        // ============================================================
        registry.add(std::make_unique<ClearCommand>());

        // ============================================================
        // FIN ZONA DE EQUIPOS
        // ============================================================

        return registry;
    }

} // namespace CyberpunkCba
