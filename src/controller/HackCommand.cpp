#include "HackCommand.h"
#include <iostream>
using namespace std;

class HackCommand : public Command
{
    public:
    void Execute(GameModel& model) override
    bool casoBorde = false;
    {
        if (model.credits() >= model.hackCost() && model.hackAttempts() > 0)
        {
            cout << "OK" << endl;
        }
        else if model.credits() < model.hackCost() && model.hackAttempts() == 0){
            cout << "Sin creditos ni intentos" << endl;
            casoBorde = true;
        }
        else if (model.credits() < model.hackCost() && basoBorde == false)
        {
            cout << "Sin creditos" << endl;
        }
        else if (model.hackAttempts() == 0 && casoBorde == false)
        {
            cout << "Sin intentos" << endl;
        }
    }
};