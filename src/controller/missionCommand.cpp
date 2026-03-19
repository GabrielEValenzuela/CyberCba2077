#include "missionCommand.hpp"
#include "model/gameModel.hpp"           // para el model
#include "common/types.hpp"             // para los datos de la misión
#include <iostream>
#include <iomanip>

namespace CyberpunkCba
{
    void MissionCommand::execute(GameModel& model)
    {
        const Mission* active = model.activeMission();
        const Mission* lastFailed = model.lastFailedMission();

        // Tres estados de salida
        if (active != nullptr)
        {
            std::cout << ">>> Misión Actual: " << active->getName() <<std::endl;
            renderBar(active->getProgress(), 20);
        }
        else if (lastFailed != nullptr)
        {
            std::cout << "Aviso: Ultimo fallo en " << lastFailed->getName() << std::endl;
        }
        else
        {
            std::cout << "Aviso: Sin misiones."<<std::endl;
        }

        //AGREGAR LA FUNCION CALCULATESUCCESSRATE¿?
    }
}