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
        bool esSoloBlancos = true; // declaro boolenao para saber si el input es solo espacios en blanco
        for (std:: size_t i =0; i<m_input.size(); i++){
            if (m_input[i] != ' ')  //si por lo menos uno es distinto de espacio
            {
                esSoloBlancos = false;   //entonces no tiene solo blancos
                break;
            }
        }
        if(esSoloBlancos){
            std::cout << COLOR_RED << "  Comando en blanco.\n"
                      << COLOR_RESET << "  Escribí 'help' para ver los comandos disponibles.\n\n";  //mensaje diferente al vacio
        }
        //caso comando desconocido
        // como la consigna dice que la sugerencia debe venir de los dos primeros caracteres iguales
        // entonces, primero nos aseguramos de que el input tenga al menos 2 caracteres 
        if(m_input.size() >=2){
            std:: string sugerencia = m_input.substr(0,2); // el substring de los dos primeros caracteres del input
            for( const Command* cmd : m_registry.commands()){   // por cada puntero cmd a la lista de comanddos del registry
                 std:: string nombreComando = cmd->name();       // guardo el nombre del comando :P
                 if(nombreComando.size() >= 2 && nombreComando.substr(0,2) == sugerencia){ //nos aseguramos que el comando tenga almenos dos caracteres y los comparamos con el de la sugerencia
                    std::cout << COLOR_RED << " ¿Quizás quisite decir... \"" << nombreComando << "\"?\n"
                      << COLOR_RESET;
                    break;
                 }

            }

        }

    

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
