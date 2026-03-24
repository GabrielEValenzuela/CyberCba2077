#ifndef _CLEARCOMMAND_HPP
#define _CLEARCOMMAND_HPP

#include "controller/command.hpp"
#include <string>

/**
 * @file clearCommand.hpp
 * @brief Limpia la terminal con ANSI escapes
 *        y renderiza un header post-limpieza con contenido dinámico
 *        derivado del modelo
 **/

namespace CyberpunkCba
{
    class ClearCommand : public Command
    {
    public:
        ClearCommand() = default;
        ~ClearCommand() override = default;

        void execute(GameModel& model) override;
        std::string name() const override;
        std::string description() const override;
        std::string category() const override;

    private:
        std::string renderHeader(const GameModel& model) const;
    };
} // namespace CyberpunkCba

#endif // _CLEARCOMMAND_HPP
