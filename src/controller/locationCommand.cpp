#include "controller/locationCommand.hpp"
#include "model/gameModel.hpp"
#include "common/types.hpp"
#include <iostream>
#include <string>

namespace CyberpunkCba
{
void locationCommand::execute(GameModel& model)
{
    const std::string& zone = model.currentZone();

    if (zone.empty() || zone == "unknown")
    {
        std::cout << "Señal perdida." << std::endl;
    }

    const std::string connectivity = (zone.find("Corporativo") != std::string::npos) ? "RED PRIVADA" : "RED PUBLICA";

    const auto& exits = model.adjacentZones();

    std::cout << "UBICACION DEL RUNNER" << std::endl;
    std::cout << "Zona : " << zone << std::endl;
    std::cout << "Red : " << connectivity << std::endl;
    std::cout << "Peligro : " << buildDangerBar(model.alertLevel()) << std::endl;
    std::cout << "Hora del dia : " << timeOfDayToString(model.timeOfDay()) << std::endl;
    std::cout << std::endl;
    std::cout << "Salidas : " << std::endl;

    if (exits.empty())
    {
        std::cout << "Sin salidas conocidas" << std::endl;
    }
    else
    {
        for (const auto& exit : exits)
        {
            std::cout << " -> " << exit << std::endl;
        }
    }

    std::cout << std::endl;
}

std::string locationCommand::name() const
{
    return "location";
}

std::string locationCommand::description() const
{
    return "Muestra zona actual, peligro y zonas adyacentes.";
}

std::string locationCommand::category() const
{
    return "mundo";
}

std::string locationCommand::buildDangerBar(AlertLevel level) const
{
    const int totalBlocks = 10;
    const int levelValue = static_cast<int>(level);
    const int filled = levelValue * totalBlocks / 4;

    std::string bar = "[";
    bar += std::string(static_cast<size_t>(filled),'#');
    bar += std::string(static_cast<size_t>(totalBlocks - filled), '.');
    bar += "] ";
    bar += alertLevelToString(level);

    return bar;
}

}