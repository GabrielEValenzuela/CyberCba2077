#pragma once

#include "../../include/controller/command.hpp"
#include <string>

namespace CyberpunkCba
{
    class logCommand : public Command
    {
    public:
        [[nodiscard]] std::string name() const override;
        [[nodiscard]] std::string description() const override;
        [[nodiscard]] std::string category() const override;

        void execute(GameModel& model) override;
    };
}