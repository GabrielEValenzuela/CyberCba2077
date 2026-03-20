#include "unknownCommand.hpp"
#include "controller/commandRegistry.hpp" // para usar CommandRegistry en el constructor y en execute().
#include <iostream>

namespace CyberpunkCba
{

    static constexpr std::string_view COLOR_RED {"\033[31m"};
    static constexpr std::string_view COLOR_RESET {"\033[0m"};

    UnknownCommand::UnknownCommand(std::string input, const CommandRegistry& registry) //actualizar el constructor para recibir la referencia a la tabla de comandos, y guardarla en el atributo m_registry.
        : m_input {std::move(input)}, m_registry {registry}
    {
    }

    void UnknownCommand::setInput(std::string input)
    {
        m_input = std::move(input);
    }

    void UnknownCommand::execute(GameModel& /*model*/)
    {
        if (m_input.empty())
        {
            return;
        }
        std::cout << COLOR_RED << "  Comando desconocido: \"" << m_input << "\"\n"
                  << COLOR_RESET << "  Escribí 'help' para ver los comandos disponibles.\n\n";
        
        // caso white-only
        
    }

    std::string UnknownCommand::name() const
    {
        return "__unknown";
    }
    std::string UnknownCommand::description() const
    {
        return "";
    }
    std::string UnknownCommand::category() const
    {
        return "__internal";
    }

} // namespace CyberpunkCba
