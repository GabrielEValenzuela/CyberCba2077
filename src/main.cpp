#include "model/GameModel.hpp"
#include "model/mercenary.hpp"
#include "model/squad.hpp"

#include <iostream>

int main()
{
    constexpr int InitialCredits = 50;

    cyberpunk::GameModel model;

    model.addCredits(InitialCredits);

    auto theMercs = new Squad<Mercenary>("Los mercenarios de NvaCba");
    theMercs->enlistToSquad(Mercenary("William",100.0f,78));

    //theMercs->showAll();

    std::cout << "Cyberpunk Córdoba 2077 boot OK. Credits=" << model.credits() << "\n";
    return 0;
}
