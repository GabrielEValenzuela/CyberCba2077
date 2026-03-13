#include "controller/commandRegistry.hpp"
#include "model/gameModel.hpp"
#include "ui/console.hpp"
#include <iostream>

/**
 * @file main.cpp
 * @brief Punto de entrada de Cyberpunk Córdoba 2077.
 *
 * @details
 * Construcción por valor → RAII completo.
 * Sin new, sin delete, sin punteros en main.
 *
 * @author Exodus Systems Inc.
 * @version 0.1.0
 */
auto constexpr DEFAULT_PLAYER_NAME {"Runner_001"};

int main()
{
    CyberpunkCba::GameModel model {DEFAULT_PLAYER_NAME};
    CyberpunkCba::CommandRegistry registry {CyberpunkCba::buildRegistry()};
    CyberpunkCba::Console console {model, registry};

    console.run();

    return 0;
}
