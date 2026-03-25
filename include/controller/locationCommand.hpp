#ifndef _LOCATION_COMMAND_HPP
#define _LOCATION_COMMAND_HPP

#include "command.hpp"
#include "common/types.hpp"
#include <string>

/**
 * @file locationCommand.hpp
 * @brief Comando que muestra la ubicacion de Runner
 * @details loocationCommand el gameModel para mostrar el estado de la zona actual
 * @author Equipo MMMS
 */

namespace CyberpunkCba
{

    /**
     * @class locationCommand
     * @brief Muestra la ubicacion, peligro, red y salidas de la zona actual
     */

    class locationCommand : public Command
    {
    public:
        /// @brief Constructor por defecto
        locationCommand() = default;

        /// @brief Destructor por defecto
        ~locationCommand() override = default;

        /**
         * @brief Muesstra la ubicacion del Runner por consola
         */
        void execute(GameModel& model) override;

        /**
         * @brief Identificador del comando
         * @return "location"
         */
        std::string name() const override;

        /**
         * @brief Descripcion corta para el menu de ayuda
         * @return String no vacio
         */
        std::string description() const override;

        /**
         * @brief Categoria del comando
         * @return "mundo"
         */
        std::string category() const override;

    private:
        /**
         * @brief Genera una barra visual del nivel del peligro
         * @return String con la barra formateada, no vacio
         */
        std::string buildDangerBar(AlertLevel level) const;
    };

} // namespace CyberpunkCba

#endif // _LOCATION_COMMAND_HPP