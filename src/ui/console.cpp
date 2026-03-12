#include "ui/console.hpp"
#include "common/types.hpp"
#include "common/version.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>

namespace CyberpunkCba
{

    Console::Console(GameModel& model, CommandRegistry& registry) noexcept
        : m_model {model}
        , m_registry {registry}
    {
    }

    void Console::run()
    {
        printBanner();

        while (m_model.isRunning())
        {
            printPrompt();

            const auto input {readLine()};

            if (!input.has_value())
            {
                // EOF — Ctrl+D
                std::cout << "\n";
                m_model.quit();
                break;
            }

            const auto commandName {parseCommandName(*input)};

            auto& cmd {m_registry.dispatch(commandName)};
            cmd.execute(m_model);
            m_model.incrementCommandCount();
        }

        std::cout << COLOR_CYAN << "\n  [EXODUS SYSTEMS] Sesión terminada. "
                  << "Comandos ejecutados: " << m_model.commandCount() << "\n\n"
                  << COLOR_RESET;
    }

    // =============================================================================
    // Privados
    // =============================================================================

    void Console::printBanner() const
    {
        std::cout << COLOR_CYAN << "\n"
                  << "  ╔═══════════════════════════════════════════╗\n"
                  << "  ║     CYBERPUNK CÓRDOBA 2077 v" << VERSION_STRING << "         ║\n"
                  << "  ║     Exodus Systems Inc. — Trainee Program ║\n"
                  << "  ╚═══════════════════════════════════════════╝\n"
                  << COLOR_RESET << "\n"
                  << "  Runner identificado: " << COLOR_YELLOW << m_model.playerName() << COLOR_RESET << "\n"
                  << "  Escribí 'help' para ver los comandos.\n\n";
    }

    void Console::printPrompt() const
    {
        const auto color {alertColor(m_model.alertLevel())};
        std::cout << color << "[" << m_model.playerName() << " | " << m_model.credits() << "cr" << " | "
                  << alertLevelToString(m_model.alertLevel()) << "]" << COLOR_RESET << "> ";
    }

    std::optional<std::string> Console::readLine() const
    {
        std::string line;
        if (!std::getline(std::cin, line))
        {
            return std::nullopt;
        }

        // Trim whitespace
        const auto first {line.find_first_not_of(" \t\r\n")};
        if (first == std::string::npos)
        {
            return std::string {};
        }
        const auto last {line.find_last_not_of(" \t\r\n")};
        return line.substr(first, last - first + 1);
    }

    std::string Console::parseCommandName(const std::string& input) const
    {
        if (input.empty())
        {
            return {};
        }

        std::istringstream ss {input};
        std::string token;
        ss >> token;

        std::transform(
            token.begin(), token.end(), token.begin(), [](const unsigned char c) { return std::tolower(c); });

        return token;
    }

    std::string_view Console::alertColor(const AlertLevel level) noexcept
    {
        switch (level)
        {
            case AlertLevel::None: return COLOR_GREEN;
            case AlertLevel::Low: return COLOR_GREEN;
            case AlertLevel::Medium: return COLOR_YELLOW;
            case AlertLevel::High: return COLOR_RED;
            case AlertLevel::Maximum: return COLOR_BLINK;
            default: return COLOR_RESET;
        }
    }

} // namespace CyberpunkCba
