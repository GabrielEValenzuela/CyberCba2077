#include "model/GameModel.hpp"
#include <iostream>

int main()
{
    constexpr int InitialCredits = 50;

    cyberpunk::GameModel model;

    model.addCredits(InitialCredits);

    std::cout << "Cyberpunk Córdoba 2077 boot OK. Credits=" << model.credits() << "\n";
    return 0;
}
