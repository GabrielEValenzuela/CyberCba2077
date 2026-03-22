//
// Created by Shadow on 21/03/2026.
//

#include "InventoryCommand.h"
#include "model/gameModel.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace CyberpunkCba
{

    std::string InventoryCommand::name() const
    {
        return "inventario";
    }

    std::string InventoryCommand::description() const
    {
        return "Muestra el inventario del jugador, su valor total y su capacidad actual.";
    }

    std::string InventoryCommand::category() const
    {
        return "runner";
    }

    double InventoryCommand::calcularValorTotal(const std::vector<Item>& inventario) const
    {
        double valorTotal = 0.0;
        for (const auto& item : inventario)
        {
            // Cast a double para evitar errores en la multiplicación price * quantity
            valorTotal += static_cast<double>(item.price) * item.quantity;
        }
        return valorTotal;
    }
std::string InventoryCommand::renderBar(int actual, int maximo, int ancho) const
    {
        if (maximo <= 0) return "[..........]";

        int cantidadCuadraditos = (actual * ancho) / maximo;

        std::string barra = "[";
        for (int i = 0; i < ancho; i++)
        {
            if (i < cantidadCuadraditos)
                barra += "#"; // Relleno según lo que está ocupado
            else
                barra += "."; // Espacio vacío restante
        }
        barra += "]";
        return barra;
    }

    void InventoryCommand::execute(GameModel& model)
    {
        const std::vector<Item>& inv = model.inventory();
        int itemsTotal               = static_cast<int>(inv.size());
        int limite                   = model.inventoryCapacity();

        // 1. Inventario vacío
        if (itemsTotal == 0)
        {
            std::cout << "[VACIO] Tu mochila esta vacia, no tienes nada." << std::endl;
        }
        else
        {
            std::cout << "--- Inventario de " << model.playerName() << " ---" << std::endl;

            for (size_t i = 0; i < inv.size(); i++)
            {
                std::cout << "- " << inv[i].name << " x" << inv[i].quantity << std::endl;
            }
        }
        // 2. Valor total
        double valor = calcularValorTotal(inv);
        std::cout << "Valor total: " << valor << " creditos" << std::endl;

        // 3. Barra de carga (ancho de 15 caracteres)
        std::string laBarra = renderBar(itemsTotal, limite, 15);
        std::cout << "Carga actual: " << laBarra << std::endl;

        // 4. Inventario lleno
        if (itemsTotal >= limite)
        {
            std::cout << "[CUIDADO] Ya no te entra nada mas en la mochila." << std::endl;
        }
    }

} // namespace CyberpunkCba