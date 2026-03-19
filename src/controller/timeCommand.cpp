#include "timeCommand.hpp"
#include "common/types.hpp"
#include "model/gameModel.hpp"
#include <iostream>
namespace CyberpunkCba
{

    std::string TimeCommand::name() const
    {
        return "time";
    }
    std::string TimeCommand::description() const
    {
        return "Devuelve turno y hora.";
    }
    std::string TimeCommand::category() const
    {
        return "sistema";
    }

    void TimeCommand::execute(GameModel& model)
    {
        if (model.currentHour() == 0 && model.currentMinute() == 0)
        {
            std::cout << "Nuevo día de simulación iniciado.";
        }
        else
        {
            std::cout << " Hora: " << model.currentHour() << ":" << model.currentMinute()
                      << " | Turno: " << timeOfDayToString(timeOfDayFromHour(model.currentHour()))
                      << " | Tiempo restante hasta próximo turno: "
                      << minutesUntilNextTurn(model.currentHour(), model.currentMinute()) << " minutos." << std::endl;
        }
    }
} // namespace CyberpunkCba