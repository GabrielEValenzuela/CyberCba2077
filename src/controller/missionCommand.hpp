//
// Created by lucii on 19/3/26.
//
#ifndef _MISSION_COMMAND_HPP
#define _MISSION_COMMAND_HPP

#include "controller/command.hpp"
#include <string>

namespace CyberpunkCba 
{
    // Optimizacion: esto evita usar #include para gameModel.hpp, reduciendo tiempos de compilacion.
    class GameModel;

    // clase heredada
    class MissionCommand : public Command 
    {
    public:
        // Constructor y destructor explícitos en default, lo explicital command.hpp
        MissionCommand() = default;
        ~MissionCommand() override = default;

        // Métodos heredados con override
        void execute(GameModel& model) override;
        std::string name() const override;
        std::string description() const override;
        std::string category() const override;

    private:
        // Métodos privados solicitados en el issue
        float calculateSuccessRate(int completed, int failed) const;
        void renderBar(int percent, int width) const;
    };

}

#endif