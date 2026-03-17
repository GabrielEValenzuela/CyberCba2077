#include "controller/shopCommand.hpp"

#include <iostream>
#include <vector>

namespace CyberpunkCba
{
    // Implementación de ShopCommand
    //=============================================================================
    // Constructor
    //=============================================================================

    ShopCommand::ShopCommand()
    {
        getShopInventory = {
            {"Cyberdeck Upgrade", 100},
            {"Neural Implant", 150},
            {"Stealth Cloak", 200},
            {"Combat Drone", 250}
        };
    }

}

