#include "HackCommand.h"
#include "model/gameModel.hpp"
#include <iostream>

namespace CyberpunkCba
{
    std::string HackCommand::name() const
    {
        return "hack";
    }
    std::string HackCommand::description() const
    {
        return "Verifica disponibilidad de hackeo.";
    }
    std::string HackCommand::category() const
    {
        return "runner";
    }
	std::string HackCommand::difficultyBar() const
	{
		switch (m_difficulty)
	{
    case HackDifficulty::EASY: return "[■□□] FACIL";
    case HackDifficulty::MEDIUM: return "[■■□] MEDIO";
    case HackDifficulty::HARD: return "[■■■] DIFICIL";
	}
	return "[■■□] MEDIO";
	}
	void HackCommand::execute(GameModel& model)
	{
    	bool sinCreditos = model.credits() < model.hackCost();
		bool sinIntentos = model.hackAttempts() == 0;

			if (sinCreditos && sinIntentos)
			{
				std::cout << "SIN CREDITOS NI INTENTOS" << std::endl;
			}
			else if (sinCreditos&& !sinIntentos)
			{
        	std::cout << "SIN CREDITOS" << std::endl;
			}
			else if (!sinCreditos && sinIntentos)
			{
			std::cout << "SIN INTENTOS" << std::endl;
			}
			else if (!sinCreditos && !sinIntentos)
			{
			std::cout << "OK\n" << difficultyBar() << std::endl;
			}
	}
}