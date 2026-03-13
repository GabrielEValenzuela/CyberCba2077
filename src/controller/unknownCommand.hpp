#ifndef _UNKNOWN_COMMAND_HPP
#define _UNKNOWN_COMMAND_HPP

#include "controller/command.hpp"
#include <string>

/**
 * @file UnknownCommand.hpp
 * @brief Header privado de UnknownCommand. No incluir fuera de UnknownCommand.cpp.
 *
 * @details
 * Fallback de dispatch(). Nunca falla, siempre ejecutable.
 * Guarda el input original para mostrarlo en el mensaje de error.
 *
 * @author Exodus Systems Inc. (instructor)
 * @version 0.1.0
 */

namespace CyberpunkCba
{

    class UnknownCommand final : public Command
    {
    public:
        /**
         * @brief Construye con el input que generó el fallo.
         * @param input Input original del usuario.
         */
        explicit UnknownCommand(std::string input);
        ~UnknownCommand() override = default;

        /// @brief Actualiza el input para la siguiente ejecución.
        void setInput(std::string input);

    private:
        void execute(GameModel& model) override;
        std::string name() const override;
        std::string description() const override;
        std::string category() const override;

        std::string m_input;
    };

} // namespace CyberpunkCba

#endif // _UNKNOWN_COMMAND_HPP
