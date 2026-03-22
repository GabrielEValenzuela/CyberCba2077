//
// Created by diego on 22/3/26.
//

#ifndef _LOG_COMMAND_HPP
#define _LOG_COMMAND_HPP

#include <string>
#include <chrono>
#include"controller/command.hpp"

namespace CyberpunkCba
{
    class logCommand final: public Command{

    public:
        logCommand()=default;


        ~logCommand() override = default;


    private:

        std::string name() const override;

        std::string description() const override;
        std::string category() const override;
        std::string formatTimestamp(int hour, int minute) const;
        std::string formatDuration(std::chrono::seconds duration) const;
        void execute(GameModel& model) override;
    };
}
#endif // _LOG_COMMAND_HPP
