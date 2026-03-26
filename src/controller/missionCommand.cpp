#include "missionCommand.hpp"
#include "model/gameModel.hpp"           // para el model
#include "common/types.hpp"             // para los datos de la misión
#include <iostream>
#include <iomanip>

namespace CyberpunkCba
{
    std::string MissionCommand::name() const { return "mission"; }
    std::string MissionCommand::description() const { return "Muestra el estado de misiones."; }
    std::string MissionCommand::category() const { return "Status"; }

    void MissionCommand::execute(GameModel& model)
    {
        // raw pointers sin ownership
        const Mission* active = model.activeMission();
        const Mission* lastFailed = model.lastFailedMission();


        // pedimos los int del gameModel
        int terminadas = model.completedMissions();
        int falladas = model.failedMissions();

        std::cout << ">>> Tasa de éxito: " << calculateSuccessRate(terminadas, falladas) << "%\n";

        // Tres estados de salida
        if (active != nullptr)
        {
            std::cout << ">>> Misión Actual: " << active->name << std::endl;
            renderBar(active->progressPercent, 20);
        }
        else if (lastFailed != nullptr)
        {
            std::cout << "Aviso: Ultimo fallo en " << lastFailed->name << std::endl;
        }
        else
        {
            std::cout << "Aviso: Sin misiones."<< std::endl;
        }

    }

    // succesRate

    float MissionCommand::calculateSuccessRate(int completed, int failed) const
    {
        int total = completed + failed;

        if (total == 0)
        {
            return 0.0f;

        }
        else
        {
            return ((completed * 100.0f) / total);
        }
    }

    // renderbar

}