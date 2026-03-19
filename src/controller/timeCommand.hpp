#ifndef CYBERPUNK_CORDOBA_2077_TIMECOMMAND_HPP
#define CYBERPUNK_CORDOBA_2077_TIMECOMMAND_HPP
#include "controller/command.hpp"

/*
 * @class TimeCommand
 * @brief Muestra la hora de simulación, el turno y el tiempo restante hasta el próximo turno.
 * @author lorem ipsum Team
 */
namespace CyberpunkCba
{

    class TimeCommand final : public Command
    {
    public:
        TimeCommand() = default;
        ~TimeCommand() override = default;
        void execute(GameModel& model) override;
        [[nodiscard]] std::string name() const override;
        [[nodiscard]] std::string description() const override;
        [[nodiscard]] std::string category() const override;
    };

} // namespace CyberpunkCba

#endif // CYBERPUNK_CORDOBA_2077_TIMECOMMAND_HPP
