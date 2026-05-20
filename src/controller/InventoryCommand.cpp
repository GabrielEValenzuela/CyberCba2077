//
// Created by Shadow on 21/03/2026.
//
#include "InventoryCommand.hpp"
#include "model/gameModel.hpp"

#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace CyberpunkCba
{

namespace
{
    int safeParseInt(const std::string& s)
    {
        try
        {
            return std::stoi(s);
        }
        catch (const std::invalid_argument&)
        {
            return -1;
        }
        catch (const std::out_of_range&)
        {
            return -1;
        }
    }

    std::vector<Item> loadInventoryFromFile(const std::string& path)
    {
        std::vector<Item> result;
        std::ifstream     file(path);

        if (!file.is_open())
        {
            return result;
        }

        std::string line;
        while (std::getline(file, line))
        {
            if (line.empty()) continue;

            std::istringstream ss(line);
            std::string        name, typeStr, priceStr, quantityStr;

            if (!std::getline(ss, name,        ',')) continue;
            if (!std::getline(ss, typeStr,     ',')) continue;
            if (!std::getline(ss, priceStr,    ',')) continue;
            if (!std::getline(ss, quantityStr, ',')) continue;

            Item item;
            item.name     = name;
            item.type     = ItemType::Value;
            item.price    = safeParseInt(priceStr);
            item.quantity = safeParseInt(quantityStr);

            result.push_back(item);
        }

        return result;
    }

} // namespace

int InventoryCommand::inventoryValue(const std::vector<Item>& inventory,
                                     std::size_t              index)
{
    if (index == inventory.size())
    {
        return 0;
    }

    const Item& item = inventory[index];

    if (item.price < 0 || item.quantity < 0)
    {
        return inventoryValue(inventory, index + 1);
    }

    return item.price * item.quantity + inventoryValue(inventory, index + 1);
}

std::string InventoryCommand::name() const
{
    return "inventario";
}

std::string InventoryCommand::description() const
{
    return "Muestra el inventario del jugador y su valor total en creditos.";
}

std::string InventoryCommand::category() const
{
    return "runner";
}

void InventoryCommand::execute(GameModel& model)
{
    const std::vector<Item> inv = loadInventoryFromFile("inventory.txt");

    if (inv.empty())
    {
        std::cout << "[VACIO] Tu mochila esta vacia, no tienes nada." << std::endl;
    }
    else
    {
        std::cout << "--- Inventario de " << model.playerName() << " ---" << std::endl;

        for (const auto& item : inv)
        {
            std::cout << "- " << item.name
                      << " x" << item.quantity << std::endl;
        }
    }

    const int valor = inventoryValue(inv, 0);
    std::cout << "Valor total: " << valor << " creditos" << std::endl;
}

} // namespace CyberpunkCba