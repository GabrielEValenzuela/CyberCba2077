#ifndef _COMMAND_REGISTRY_HPP
#define _COMMAND_REGISTRY_HPP

#include "controller/command.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @file CommandRegistry.hpp
 * @brief Registro central de comandos — capa Controller del MVC.
 *
 * @details
 * Único punto de registro y dispatch de comandos. Almacena comandos
 * como unique_ptr<Command> y los despacha por nombre en O(1).
 *
 * ## Invariantes de clase
 * - No hay dos comandos con el mismo name() registrados simultáneamente.
 * - dispatch() nunca retorna nullptr.
 * - El orden de iteración en commands() es el orden de registro.
 *
 * ## Política
 * READ ONLY para equipos de alumnos.
 * Para registrar un comando, agregar una línea en la sección delimitada
 * de CommandRegistry.cpp.
 *
 * @author Exodus Systems Inc.
 * @version 0.1.0
 */

namespace CyberpunkCba
{

    class CommandRegistry final
    {
    public:
        // =========================================================================
        /// @name Construcción
        // =========================================================================
        /// @{

        /// @brief Construye registry vacío.
        CommandRegistry() = default;

        /// @brief No copiable — ownership exclusivo de comandos.
        CommandRegistry(const CommandRegistry&) = delete;
        /// @brief No copiable.
        CommandRegistry& operator=(const CommandRegistry&) = delete;

        /// @brief Movible.
        CommandRegistry(CommandRegistry&&) = default;
        /// @brief Movible.
        CommandRegistry& operator=(CommandRegistry&&) = default;

        ~CommandRegistry() = default;

        /// @}

        // =========================================================================
        /// @name Registro
        // =========================================================================
        /// @{

        /**
         * @brief Registra un comando. Toma ownership via unique_ptr.
         *
         * @details Si ya existe un comando con el mismo name(), lo reemplaza.
         *
         * @param spCommand Comando a registrar. No debe ser nullptr.
         * @pre  spCommand != nullptr
         * @pre  !spCommand->name().empty()
         * @post contains(spCommand->name()) == true
         */
        void add(std::unique_ptr<Command> spCommand);

        /// @}

        // =========================================================================
        /// @name Dispatch
        // =========================================================================
        /// @{

        /**
         * @brief Despacha un comando por nombre.
         *
         * @details Busca por name() exacto (case-sensitive tras normalización).
         *          Si no se encuentra, retorna UnknownCommand — nunca nullptr.
         *
         * @param input Nombre del comando ingresado por el usuario.
         * @return Referencia al comando. Válida mientras el registry exista.
         * @post Retorna comando válido y ejecutable siempre.
         */
        Command& dispatch(const std::string& input);

        /// @}

        // =========================================================================
        /// @name Consulta
        // =========================================================================
        /// @{

        /// @brief true si existe un comando con ese nombre.
        bool contains(const std::string& name) const;

        /**
         * @brief Lista de comandos en orden de registro.
         * @details Punteros observadores — no transferir ownership.
         * @return Vector de punteros no nulos.
         */
        const std::vector<const Command*>& commands() const noexcept;

        /// @brief Cantidad de comandos registrados.
        std::size_t size() const noexcept;

        /// @brief true si no hay comandos registrados.
        bool empty() const noexcept;

        /// @}

    private:
        /// @brief Mapa nombre → comando. Owner de todos los comandos.
        std::unordered_map<std::string, std::unique_ptr<Command>> m_commandMap;

        /// @brief Lista ordenada para iteración en HelpCommand.
        std::vector<const Command*> m_commandList;

        /**
         * @brief Fallback para input desconocido.
         * @details Puntero observador al UnknownCommand en m_commandMap.
         *          Nunca nullptr después de la primera llamada a add() con
         *          un UnknownCommand.
         */
        Command* m_pUnknownCommand {nullptr};
    };

    /// @brief Factory — construye y retorna el registry completo.
    /// @details Definida en CommandRegistry.cpp. Llamar una vez desde main().
    CommandRegistry buildRegistry();

} // namespace CyberpunkCba

#endif // _COMMAND_REGISTRY_HPP
