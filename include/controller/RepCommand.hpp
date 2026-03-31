#ifndef CYBERPUNK_CORDOBA_2077_REPCOMMAND_HPP
#define CYBERPUNK_CORDOBA_2077_REPCOMMAND_HPP

#include "command.hpp"             // Para heredar de Command
#include "model/gameModel.hpp"     // Para que reconozca GameModel
#include "common/types.hpp"        // Para que reconozca Faction
#include <string>

namespace CyberpunkCba {

    class RepCommand : public Command {
    public:
        RepCommand() = default;

        void execute(GameModel& model) override;
        std::string name() const override { return "reputation"; }
        std::string description() const override { return "Muestra la reputacion por facción con barras de progreso."; }
        std::string category() const override { return "runner"; }

    private:
        // El metodo privado para la barrita
        std::string renderBar(int value, int maxValue, int width) const;
        // El metodo privado para la etiqueta (¡para evitar el switch en el execute!)
        std::string renderLabel(RepLevel level) const;
    };

} // namespace CyberpunkCba

#endif