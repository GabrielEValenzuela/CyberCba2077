//
// Created by lucii on 19/3/26.
//
#ifndef MISSION_COMMAND_HPP
#define MISSION_COMMAND_HPP
#include "controller/command.hpp"
#include "model/gameModel.hpp"

class missionCommand : public CyberpunkCba::Command //hereda command
{
public:
    missionCommand() = default;
    ~missionCommand() override = default; //destructor
    void execute( CyberpunkCba::GameModel& model) override;
    std::string name() const override;
    std::string description() const override;
    std::string category() const override;
private:
    float calculateSuccessRate(int completed, int failed) const; //tasa de exito
    void renderBar(int percent, int width) const; //barra de progreso
};




#endif