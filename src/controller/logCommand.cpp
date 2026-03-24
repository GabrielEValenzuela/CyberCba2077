#include "logCommand.hpp"
#include "../../include/model/gameModel.hpp"
#include "../../include/common/types.hpp"
#include <iomanip>
#include <iostream>
#include <vector>

namespace CyberpunkCba
{
    std::string logCommand::name() const { return "log"; }

    std::string logCommand::description() const { return "Muestra el historial"; }

    std::string logCommand::category() const { return "system"; }

    void logCommand::execute(GameModel& model)
    {
        const std::vector<LogEntry>& logComplete = model.actionLog();
        size_t totalentradas1 = logComplete.size();

        size_t inicio = 0;
        if (totalentradas1 > 10)
        {
            inicio = totalentradas1 - 10;
        }

        if (totalentradas1 > 10)
        {
            std::cout << "Ultimas 10 de " << totalentradas1 << " entradas totales:" << std::endl;
            std::cout << "---------" << std::endl;
        }

        for (size_t i = inicio; i < totalentradas1; i++)
        {
            const LogEntry& entrada = logComplete[i];

            // Reemplazamos formatTimestamp por código nativo de C++
            // setw(2) y setfill('0') aseguran que los números menores a 10 tengan un '0' adelante
            std::cout << "[" << std::setfill('0') << std::setw(2) << entrada.hour << ":"
                      << std::setfill('0') << std::setw(2) << entrada.minute << "] "
                      << entrada.message << std::endl;
        }
    }
}