#ifndef _CONSOLE_HPP
#define _CONSOLE_HPP

#include "controller/commandRegistry.hpp"
#include "model/gameModel.hpp"

#include <optional>
#include <string>
#include <string_view>

/**
 * @file Console.hpp
 * @brief Loop principal de interacción — capa View del MVC.
 *
 * @details
 * Lee input, delega dispatch a CommandRegistry, muestra output.
 * No contiene lógica de juego ni modifica GameModel directamente.
 *
 * Política: READ ONLY para equipos de alumnos.
 *
 * @author Exodus Systems Inc.
 * @version 0.1.0
 */

namespace CyberpunkCba
{

    class Console final
    {
    public:
        /**
         * @brief Construye la consola con sus dependencias.
         * @param model    Estado del juego. Debe outlive Console.
         * @param registry Registry de comandos. Debe outlive Console.
         */
        Console(GameModel& model, CommandRegistry& registry) noexcept;

        Console(const Console&) = delete;
        Console& operator=(const Console&) = delete;
        Console(Console&&) = delete;
        Console& operator=(Console&&) = delete;
        ~Console() = default;

        /**
         * @brief Inicia el loop principal.
         * @details Continúa mientras model_.isRunning() == true.
         *          EOF (Ctrl+D) termina limpiamente.
         * @post m_model.isRunning() == false al retornar.
         */
        void run();

    private:
        void printBanner() const;
        void printPrompt() const;

        /// @return Input normalizado. nullopt en EOF.
        std::optional<std::string> readLine() const;

        /// @brief Primer token del input, en minúsculas.
        std::string parseCommandName(const std::string& input) const;

        /// @brief Código ANSI según AlertLevel.
        static std::string_view alertColor(AlertLevel level) noexcept;

        static constexpr std::string_view COLOR_RESET {"\033[0m"};
        static constexpr std::string_view COLOR_GREEN {"\033[32m"};
        static constexpr std::string_view COLOR_YELLOW {"\033[33m"};
        static constexpr std::string_view COLOR_RED {"\033[31m"};
        static constexpr std::string_view COLOR_BLINK {"\033[5;31m"};
        static constexpr std::string_view COLOR_CYAN {"\033[36m"};

        GameModel& m_model;
        CommandRegistry& m_registry;
    };

} // namespace CyberpunkCba

#endif // _CONSOLE_HPP
