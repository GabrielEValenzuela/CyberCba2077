#include "helpCommand.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>

namespace CyberpunkCba
{

    static constexpr std::string_view COLOR_CYAN {"\033[36m"};
    static constexpr std::string_view COLOR_YELLOW {"\033[33m"};
    static constexpr std::string_view COLOR_RESET {"\033[0m"};
    static constexpr int CMD_WIDTH {14};

    HelpCommand::HelpCommand(const CommandRegistry& registry)
        : m_registry {registry}
    {
    }

    void HelpCommand::execute(GameModel& /*model*/)
    {
        if (m_registry.empty())
        {
            std::cout << "No hay comandos registrados.\n";
            return;
        }

        std::cout << "\n"
                  << COLOR_CYAN << "╔══════════════════════════════════════╗\n"
                  << "║     CYBERPUNK CBA 2077 — COMANDOS    ║\n"
                  << "╚══════════════════════════════════════╝\n"
                  << COLOR_RESET;

        const auto grouped {groupByCategory()};

        for (const auto& [category, commandsInCategory] : grouped)
        {
            printCategory(category, commandsInCategory);
        }

        std::cout << "\n"
                  << COLOR_YELLOW << "  Total: " << m_registry.size() << " comandos disponibles.\n"
                  << COLOR_RESET << "\n";
    }

    std::string HelpCommand::name() const
    {
        return "help";
    }
    std::string HelpCommand::description() const
    {
        return "Muestra los comandos disponibles.";
    }
    std::string HelpCommand::category() const
    {
        return "sistema";
    }

    // =============================================================================
    // Privados
    // =============================================================================

    std::map<std::string, std::vector<const Command*>> HelpCommand::groupByCategory() const
    {
        std::map<std::string, std::vector<const Command*>> grouped;

        for (const auto* pCmd : m_registry.commands())
        {
            grouped[pCmd->category()].emplace_back(pCmd);
        }
        return grouped;
    }

    void HelpCommand::printCategory(const std::string& categoryName,
                                    const std::vector<const Command*>& categoryCommands) const
    {
        std::cout << "\n" << COLOR_YELLOW << "  [ " << categoryName << " ]\n" << COLOR_RESET;

        for (const auto* pCmd : categoryCommands)
        {
            std::cout << "    " << std::left << std::setw(CMD_WIDTH) << pCmd->name() << "  " << pCmd->description()
                      << "\n";
        }
    }

} // namespace CyberpunkCba
