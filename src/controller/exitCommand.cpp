#include "exitCommand.hpp"
#include "model/gameModel.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>

namespace CyberpunkCba
{

    void ExitCommand::execute(GameModel& model)
    {
        if (!model.isRunning())
        {
            std::cout << "La sesión ya ha terminado.\n";
            return;
        }

        model.quit();

        const auto duration_sec = model.sessionDuration();
        const auto hours = std::chrono::duration_cast<std::chrono::hours>(duration_sec);
        const auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration_sec % std::chrono::hours(1));
        const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration_sec % std::chrono::minutes(1));

        std::cout << "\nTerminando simulación...\n";

        if (model.isCriticalHp())
        {
            std::cout << "\nADVERTENCIA: Runner desconectado en estado crítico.\n";
            std::cout << "             Soporte vital recomendado.\n";
        }
        else
        {
            std::cout << "\nSalida segura completada.\n";
        }

        std::cout << "\n--- Métricas de Sesión ---\n";
        std::cout << "Comandos ejecutados: " << model.commandCount() << "\n";
        std::cout << "Tiempo de sesión   : " << std::setw(2) << std::setfill('0') << hours.count() << ":"
                  << std::setw(2) << std::setfill('0') << minutes.count() << ":" << std::setw(2) << std::setfill('0')
                  << seconds.count() << "\n\n";
    }

    std::string ExitCommand::name() const
    {
        return "exit";
    }

    std::string ExitCommand::description() const
    {
        return "Termina la sesión actual.";
    }

    std::string ExitCommand::category() const
    {
        return "sistema";
    }

} // namespace CyberpunkCba