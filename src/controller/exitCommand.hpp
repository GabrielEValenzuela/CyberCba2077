#ifndef CYBERPUNK_CORDOBA_2077_EXITCOMMAND_HPP
#define CYBERPUNK_CORDOBA_2077_EXITCOMMAND_HPP

#include "controller/command.hpp"

namespace CyberpunkCba
// ExitCommand tiene que vivir en el mismo namespace que Command y GameModel.
// Si no está acá, el compilador la ve como una clase distinta y desconectada.
{

    class ExitCommand : public Command
    // Ahora que estamos DENTRO de CyberpunkCba, podés escribir
    // solo "Command" en lugar de "CyberpunkCba::Command" — ya lo conoce.
    {
    public:
        void execute(GameModel& model) override;

        std::string name() const override;

        std::string description() const override;

        std::string category() const override;

        ~ExitCommand() override = default;
    };

} // namespace CyberpunkCba

#endif // CYBERPUNK_CORDOBA_2077_EXITCOMMAND_HPP