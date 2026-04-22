#ifndef _HELP_COMMAND_HPP
#define _HELP_COMMAND_HPP

#include "controller/command.hpp"
#include "controller/commandRegistry.hpp"

#include <map>
#include <string>
#include <vector>

/**
 * @file HelpCommand.hpp
 * @brief Header privado de HelpCommand.
 *
 * @details
 * Este header NO es parte de la API pública del sistema.
 * Solo debe ser incluido por HelpCommand.cpp.
 * La API pública es Command.hpp.
 *
 * @author Equipo 01 — Exodus Systems Inc.
 * @version 0.1.0
 */

namespace CyberpunkCba
{

    /**
     * @class HelpCommand
     * @brief Muestra los comandos disponibles agrupados por categoría.
     *
     * @details
     * Itera sobre CommandRegistry y agrupa los comandos por category().
     * Demuestra polimorfismo real: el mismo puntero Command* se usa para
     * llamar name(), description() y category() en distintas derivadas.
     */
    class HelpCommand final : public Command
    {
    public:
        /**
         * @brief Construye HelpCommand con referencia al registry.
         * @param registry Registry de comandos. Debe outlive HelpCommand.
         */
        explicit HelpCommand(const CommandRegistry& registry);

        ~HelpCommand() override = default;

    private:
        // Wazuh convention: implementaciones de interfaz son private.

        /**
         * @brief Agrupa y muestra los comandos por categoría.
         * @details Si el registry está vacío, muestra mensaje apropiado.
         *          Complejidad: O(n) donde n = cantidad de comandos.
         * @param model No modificado por este comando.
         */
        void execute(GameModel& model) override;
        std::string name() const override;
        std::string description() const override;
        std::string category() const override;

        /**
         * @brief Agrupa comandos por categoría.
         * @return Mapa categoría → lista de punteros observadores.
         *         Orden de categorías: alfabético.
         *         Orden de comandos por categoría: orden de registro.
         */
        std::map<std::string, std::vector<const Command*>> groupByCategory() const;

        /**
         * @brief Formatea e imprime un grupo de comandos.
         * @param categoryName Nombre de la categoría.
         * @param categoryCommands Comandos del grupo. No vacío.
         */
        void printCategory(const std::string& categoryName, const std::vector<const Command*>& categoryCommands) const;

        const CommandRegistry& m_registry; ///< Registry observado. No owned.
    };

} // namespace CyberpunkCba

#endif // _HELP_COMMAND_HPP
