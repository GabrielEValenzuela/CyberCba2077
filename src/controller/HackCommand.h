//
// Created by diego on 16/03/2026.
//

#ifndef CYBERCBA2077_HACKCOMMAND_H
#define CYBERCBA2077_HACKCOMMAND_H

#include "controller/command.hpp"
#include <string>

namespace CyberpunkCba
{
    enum class HackDifficulty { EASY, MEDIUM, HARD };
    class HackCommand : public Command
    {
    public:
        ~HackCommand() override = default;
    private:
        void execute(GameModel& model) override;
        std::string name() const override;
        std::string description() const override;
        std::string category() const override;
        std::string difficultyBar() const;
        HackDifficulty m_difficulty { HackDifficulty::MEDIUM };
    };
}
#endif // CYBERCBA2077_HACKCOMMAND_H
