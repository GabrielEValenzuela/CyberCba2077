#ifndef _STATUS_COMMAND_HPP
#define _STATUS_COMMAND_HPP

#include "controller/command.hpp"
#include <string>

/**
 * @file StatusCommand.hpp
 * @brief Header privado de StatusCommand.
 * @author Equipo 02 — Exodus Systems Inc.
 * @version 0.1.0
 */

namespace CyberpunkCba
{

    /**
     * @class StatusCommand
     * @brief Muestra el estado del runner con barras derivadas del modelo.
     */
    class StatusCommand final : public Command
    {
    public:
        StatusCommand() = default;
        ~StatusCommand() override = default;

    private:
        void execute(GameModel& model) override;
        std::string name() const override;
        std::string description() const override;
        std::string category() const override;

        /**
         * @brief Genera una barra de progreso ASCII.
         * @param value   Valor actual.
         * @param maxValue Valor máximo. Debe ser > 0.
         * @param width   Ancho de la barra en caracteres.
         * @return String con bloques ▓ y ░.
         * @pre maxValue > 0
         * @pre width > 0
         */
        std::string renderBar(int value, int maxValue, int width) const;

        /**
         * @brief Deriva el estado general del runner.
         * @param hp    HP actual.
         * @param maxHp HP máximo.
         * @return "OPERATIVO" si hp > 20% de maxHp, "EN PELIGRO" si no.
         */
        std::string deriveRunnerStatus(int hp, int maxHp) const noexcept;
    };

} // namespace CyberpunkCba

#endif // _STATUS_COMMAND_HPP
