#ifndef CYBERPUNK_CORDOBA_2077_MISSIONCOMMAND_H
#define CYBERPUNK_CORDOBA_2077_MISSIONCOMMAND_H

#include "controller/missionCommand.hpp"
#include <iostream>
#include <iomanip>

void missionCommand::execute(CyberpunkCba::GameModel& model)
{
    const CyberpunkCba::Mission* active = model.activeMission();
    const CyberpunkCba::Mission* lastFailed = model.lastFailedMission();

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
#endif // CYBERPUNK_CORDOBA_2077_MISSIONCOMMAND_H