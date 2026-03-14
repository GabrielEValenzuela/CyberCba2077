#ifndef HELP_COMMAND_HPP
#define HELP_COMMAND_HPP

#include "controller/command.hpp" 
#include "controller/commandRegistry.hpp"   
#include <map>
#include <vector>
#include <string>

namespace CyberpunkCba {

class HelpCommand : public Command {
public:
    // Constructor que recibe la referencia (no copia)
    explicit HelpCommand(const CommandRegistry& registry);

    // Los 4 métodos virtuales puros con override explícito
    void execute(GameModel& model) override;
    std::string name() const override;
    std::string description() const override;
    std::string category() const override;

private:
    // Atributo para almacenar la referencia al registro
    const CommandRegistry& m_registry;
};

} // namespace CyberpunkCba

#endif // HELP_COMMAND_HPP