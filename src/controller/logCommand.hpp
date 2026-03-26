#pragma once

#include "../../include/controller/command.hpp"
#include <string>
#include <chrono>

namespace CyberpunkCba
{
    class logCommand : public Command
    {
    public:
        [[nodiscard]] std::string name() const override;
        [[nodiscard]] std::string description() const override;
        [[nodiscard]] std::string category() const override;

        void execute(GameModel& model) override;

        // REQUISITO: Métodos privados exigidos por la Issue
    private:
        [[nodiscard]] std::string formatTimestamp(int hour, int minute) const;
        [[nodiscard]] std::string formatDuration(std::chrono::seconds duration) const;
    };
}