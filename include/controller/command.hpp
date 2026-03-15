#ifndef _COMMAND_HPP
#define _COMMAND_HPP

#include <string>

/**
 * @file Command.hpp
 * @brief Interfaz base del sistema de comandos — capa Controller del MVC.
 *
 * @details
 * Todo comando del juego hereda de esta clase e implementa los cuatro
 * métodos virtuales puros. CommandRegistry almacena comandos como
 * unique_ptr<Command> y los invoca polimórficamente.
 *
 * ## Contrato
 * - El caller garantiza que GameModel está en estado válido antes de execute().
 * - El comando garantiza que GameModel permanece válido después de execute().
 *
 * ## Ownership
 * Comandos owned exclusivamente por CommandRegistry via unique_ptr<Command>.
 * No se copian ni se mueven fuera del registry.
 *
 * ## Wazuh style
 * - Métodos de interfaz implementados en derivadas como private/protected.
 * - override explícito en todas las implementaciones.
 * - noexcept donde corresponda.
 *
 * ## Política
 * READ ONLY para equipos de alumnos.
 *
 * @author Exodus Systems Inc.
 * @version 0.1.0
 */

namespace CyberpunkCba
{

    /// Forward declaration — evita incluir GameModel.hpp en headers de comandos.
    class GameModel;

    /**
     * @class Command
     * @brief Clase base abstracta para todos los comandos del sistema.
     *
     * @note No instanciable directamente. Toda implementación concreta
     *       debe implementar los cuatro métodos virtuales puros.
     */
    class Command
    {
    public:
        // =========================================================================
        /// @name Interfaz pública
        // =========================================================================
        /// @{

        /**
         * @brief Ejecuta el comando sobre el modelo de juego.
         *
         * @details
         * Puede modificar model (ej: ScanCommand incrementa alertLevel)
         * o ser no-op sobre él (ej: HelpCommand, ClearCommand).
         * No lanza excepciones observables al caller.
         *
         * @param model Estado del juego. Válido antes y después.
         * @pre  model satisface todos sus invariantes.
         * @post model satisface todos sus invariantes.
         */
        virtual void execute(GameModel& model) = 0;

        /**
         * @brief Identificador único del comando.
         * @return String no vacío, inmutable durante el lifetime del objeto.
         *         Ejemplos: "help", "status", "scan".
         * @post Mismo valor en todas las invocaciones.
         */
        virtual std::string name() const = 0;

        /**
         * @brief Descripción de una línea para mostrar en help.
         * @return String no vacío, <= 60 caracteres.
         * @post Mismo valor en todas las invocaciones.
         */
        virtual std::string description() const = 0;

        /**
         * @brief Categoría para agrupación en help.
         * @details Valores canónicos: "sistema", "mundo", "runner", "combate".
         *          Categoría desconocida aparece en "otros".
         * @return String no vacío, inmutable durante el lifetime del objeto.
         * @post Mismo valor en todas las invocaciones.
         */
        virtual std::string category() const = 0;

        /// @}

        // =========================================================================
        /// @name Destructor
        // =========================================================================

        /**
         * @brief Destructor virtual.
         * @details Requerido para destrucción correcta a través de punteros
         *          a base. CommandRegistry almacena unique_ptr<Command>.
         */
        virtual ~Command() = default;

        // =========================================================================
        /// @name Semántica de copia y movimiento — eliminada
        // =========================================================================

        /// @brief No copiable — entidades con identidad, no valores.
        Command(const Command&) = delete;
        /// @brief No copiable.
        Command& operator=(const Command&) = delete;
        /// @brief No movible.
        Command(Command&&) = delete;
        /// @brief No movible.
        Command& operator=(Command&&) = delete;

    protected:
        /// @brief Constructor protegido — solo instanciable desde derivadas.
        Command() = default;
    };

} // namespace CyberpunkCba

#endif // _COMMAND_HPP
