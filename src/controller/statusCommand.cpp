#include "statusCommand.hpp"
#include "common/types.hpp"
#include "model/gameModel.hpp"

#include <iomanip>
#include <iostream>
#include <string>

namespace CyberpunkCba
{

    static constexpr std::string_view COLOR_GREEN {"\033[32m"};
    static constexpr std::string_view COLOR_YELLOW {"\033[33m"};
    static constexpr std::string_view COLOR_RED {"\033[31m"};
    static constexpr std::string_view COLOR_CYAN {"\033[36m"};
    static constexpr std::string_view COLOR_RESET {"\033[0m"};
    static constexpr int BAR_WIDTH {20};

    void StatusCommand::execute(GameModel& model)
    {
        const auto hpBar {renderBar(model.hp(), model.maxHp(), BAR_WIDTH)};
        const auto status {deriveRunnerStatus(model.hp(), model.maxHp())};
        const auto alertStr {alertLevelToString(model.alertLevel())};
        const auto todStr {timeOfDayToString(model.timeOfDay())};
        const auto hpColor {model.isCriticalHp() ? COLOR_RED : COLOR_GREEN};

        std::cout << "\n"
                  << COLOR_CYAN << "╔══════════════════════════════════════╗\n"
                  << "║         ESTADO DEL RUNNER            ║\n"
                  << "╚══════════════════════════════════════╝\n"
                  << COLOR_RESET << "\n"
                  << "  Runner  : " << model.playerName() << "\n"
                  << "  Estado  : " << status << "\n"
                  << "\n"
                  << "  HP      : " << hpColor << hpBar << "  " << model.hp() << "/" << model.maxHp() << COLOR_RESET
                  << "\n"
                  << "  Créditos: " << COLOR_YELLOW << model.credits() << " cr" << COLOR_RESET << "\n"
                  << "\n"
                  << "  Zona    : " << model.currentZone() << "\n"
                  << "  Alerta  : " << alertStr << "\n"
                  << "  Turno   : " << todStr << "  (" << std::setw(2) << std::setfill('0') << model.currentHour()
                  << ":" << std::setw(2) << std::setfill('0') << model.currentMinute() << ")\n"
                  << "\n"
                  << "  Comandos ejecutados: " << model.commandCount() << "\n"
                  << "\n";
    }

    std::string StatusCommand::name() const
    {
        return "status";
    }
    std::string StatusCommand::description() const
    {
        return "Muestra el estado del runner.";
    }
    std::string StatusCommand::category() const
    {
        return "runner";
    }

    // =============================================================================
    // Privados
    // =============================================================================

    std::string StatusCommand::renderBar(const int value, const int maxValue, const int width) const
    {
        const auto filled {
            static_cast<int>(static_cast<float>(value) / static_cast<float>(maxValue) * static_cast<float>(width))};

        std::string bar;
        bar.reserve(static_cast<std::size_t>(width));

        for (int i {}; i < width; ++i)
        {
            bar += (i < filled) ? "▓" : "░";
        }
        return bar;
    }

    std::string StatusCommand::deriveRunnerStatus(const int hp, const int maxHp) const noexcept
    {
        constexpr float CRITICAL_RATIO {0.20f};
        const auto threshold {static_cast<int>(static_cast<float>(maxHp) * CRITICAL_RATIO)};
        return (hp <= threshold) ? "EN PELIGRO" : "OPERATIVO";
    }

} // namespace CyberpunkCba
