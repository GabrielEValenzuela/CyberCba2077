#ifndef _SCAN_COMMAND_HPP
#define _SCAN_COMMAND_HPP

#include "controller/command.hpp"

#include <string>

namespace CyberpunkCba
{

    class ScanCommand final : public Command
    {
    public:
        ScanCommand() = default;
        ~ScanCommand() override = default;

    private:
        void execute(GameModel& model) override;
        std::string name() const override;
        std::string description() const override;
        std::string category() const override;
    };

} // namespace CyberpunkCba

#endif // _SCAN_COMMAND_HPP
