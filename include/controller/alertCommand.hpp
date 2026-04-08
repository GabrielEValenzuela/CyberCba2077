#ifndef _ALERT_COMMAND_HPP
#define _ALERT_COMMAND_HPP

// Incluimos la clase base Command. AlertCommand hereda de ella.
// Todos los comandos del juego DEBEN incluir este header.
#include "controller/command.hpp"

// Incluimos types.hpp para tener acceso al tipo AlertLevel,
// que es el enum que define los cinco niveles de alerta.
#include "common/types.hpp"

#include <string>

/**
 * @file alertCommand.hpp
 * @brief Header privado de AlertCommand.
 *
 * @details
 * Este header NO es parte de la API pública del sistema.
 * Solo debe ser incluido por alertCommand.cpp.
 * La API pública es Command.hpp.
 *
 * @author Tu equipo — Exodus Systems Inc.
 * @version 0.1.0
 */

namespace CyberpunkCba
{

    /**
     * @class AlertCommand
     * @brief Muestra el estado de alerta actual de la zona.
     *
     * @details
     * Presenta cinco niveles de alerta con mensajes diferenciados.
     * El campo "Tiempo para despeje" solo aparece en niveles >= Medium.
     * Las patrullas activas se calculan como alertLevel * 2.
     * No modifica el GameModel.
     */

    // "final" significa que nadie puede heredar de AlertCommand.
    // ": public Command" significa que AlertCommand ES un Command
    // y hereda su interfaz (los cuatro métodos virtuales puros).
    class AlertCommand final : public Command
    {
    public:

        // Constructor por defecto. No necesita parámetros porque
        // AlertCommand no almacena referencias externas (a diferencia
        // de HelpCommand que necesita el registry).
        AlertCommand() = default;

        // El destructor virtual heredado de Command se mantiene
        // con override para ser explícitos, como pide el issue.
        ~AlertCommand() override = default;

    private:

        // =====================================================================
        // Convención del proyecto (Wazuh style):
        // Los cuatro métodos de la interfaz se implementan como PRIVADOS
        // en las clases derivadas. Esto no impide que funcionen cuando
        // se llaman a través de un puntero Command* (polimorfismo).
        // La palabra "override" le dice al compilador que este método
        // reemplaza uno virtual de la clase base — si te equivocás en
        // el nombre, el compilador te avisa.
        // =====================================================================

        /**
         * @brief Muestra el estado de alerta completo.
         * @details Lee alertLevel() y lastAlertCause() del modelo.
         *          No modifica el modelo (no llama ningún mutador).
         * @param model Estado del juego. Solo lectura.
         */
        void execute(GameModel& model) override;

        /**
         * @brief Retorna el nombre del comando tal como lo tipea el jugador.
         * @return "alert"
         */
        std::string name() const override;

        /**
         * @brief Retorna la descripción corta para el menú de ayuda.
         * @return Una línea <= 60 caracteres.
         */
        std::string description() const override;

        /**
         * @brief Retorna la categoría para agrupar en el menú de ayuda.
         * @return "mundo"
         */
        std::string category() const override;

        // =====================================================================
        // Método auxiliar privado (requerido por el issue).
        //
        // adviceForLevel() centraliza toda la lógica de "qué consejo mostrar
        // según el nivel". De esta forma, execute() queda limpio y sin switch.
        //
        // "const" al final significa que este método NO puede modificar
        // ningún atributo del objeto — es una promesa al compilador.
        // =====================================================================

        /**
         * @brief Retorna el consejo táctico correspondiente al nivel de alerta.
         * @param level Nivel de alerta a consultar.
         * @return String con el consejo. Nunca vacío.
         */
        std::string adviceForLevel(AlertLevel level) const;
    };

} // namespace CyberpunkCba

#endif // _ALERT_COMMAND_HPP