#include "clearCommand.hpp"
#include "common/types.hpp"
#include "model/gameModel.hpp"
#include <iostream>

namespace CyberpunkCba
{
    void ClearCommand::execute(GameModel& model)
    {
        std::cout << "\033[2J\033[H";
        std::cout << renderHeader(model) << "\n";
    }

    std::string ClearCommand::renderHeader(const GameModel& model) const
    {
        std::string header = model.playerName();
        if (model.isCriticalHp())
        {
            header += "[HP CRITICO]";
        }
        else if (model.alertLevel() >= AlertLevel::High)
        {
            header += "[ALERTA ALTA]";
        }
        return header;
    }

    std::string ClearCommand::name() const
    {
        return "clear";
    }

    std::string ClearCommand::description() const
    {
        return "Limpia la pantalla y muestra el estado del runner.";
    }

    std::string ClearCommand::category() const
    {
        return "sistema";
    }

} // namespace CyberpunkCba
