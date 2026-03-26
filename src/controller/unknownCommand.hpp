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
    class CommandRegistry;  //avisarle al compilador que existe CommandRegistry, para usarlo en los comentarios de CommandRegistry::dispatch().

    class UnknownCommand final : public Command
    {
    public:
        /**
         * @brief Construye con el input que generó el fallo.
         * @param input Input original del usuario.
         * @param registry Referencia a la tabla de comandos.
         */
        explicit UnknownCommand(std::string input, const CommandRegistry& registry);
        ~UnknownCommand() override = default;

        /// @brief Actualiza el input para la siguiente ejecución.
        void setInput(std::string input);

    private:
        void execute(GameModel& model) override;
        std::string name() const override;
        std::string description() const override;
        std::string category() const override;

        std::string m_input;
        const CommandRegistry& m_registry; //guardo la referencia a la tabla de comandos para mostrarla en el mensaje de error, si es que el usuario escribió un comando similar a uno conocido.
    };

} // namespace CyberpunkCba

#endif // _UNKNOWN_COMMAND_HPP
