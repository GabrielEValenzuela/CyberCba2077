#include "HackCommand.hpp"
#include "model/gameModel.hpp"

#include <iostream>
#include <string>

namespace CyberpunkCba
{

    std::string HackCommand::name() const
    {
        return "hack";
    }

    std::string HackCommand::description() const
    {
        return "Verifica disponibilidad de hackeo: créditos e intentos";
    }

    std::string HackCommand::category() const
    {
        return "runner";
    }

    void HackCommand::execute(GameModel& model)
    {
        const bool sinCreditos = model.credits() < model.hackCost();
        const bool sinIntentos = model.hackAttempts() == 0;

        if (sinCreditos && sinIntentos)
        {
            std::cout << "SIN CREDITOS NI INTENTOS\n";
            return;
        }

        if (sinCreditos)
        {
            std::cout << "SIN CREDITOS\n";
            return;
        }

        if (sinIntentos)
        {
            std::cout << "SIN INTENTOS\n";
            return;
        }

        std::cout << "OK\n";
        std::cout << difficultyBar(HackDifficulty::MEDIUM) << "\n";
    }

    std::string HackCommand::difficultyBar(const HackDifficulty difficulty) const
    {
        switch (difficulty)
        {
            case HackDifficulty::EASY: return "[■□□]";
            case HackDifficulty::MEDIUM: return "[■■□]";
            case HackDifficulty::HARD: return "[■■■]";
        }

        // Inalcanzable, pero necesario para -Werror=switch y -Wreturn-type
        return "[■■□□]";
    }

} // namespace CyberpunkCba
