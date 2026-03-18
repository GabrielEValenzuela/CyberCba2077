//
// Created by gvalenzuela on 3/18/26.
//

#include "common/inventoryUtils.hpp"

#include <cassert>

namespace CyberPunkCba
{
    int inventoryValueRec(const std::vector<CyberpunkCba::Item>& inventory, const std::size_t index)
    {
        assert(index <=
               inventory.size()); // Me protejo ante la posibilidad que esta funcion se llame con un indice incorrecto

        // Caso base: Procesamos todos los items - El valor restante es 0
        if (index == inventory.size())
        {
            return 0;
        }

        // Caso recursivo: Valor del item mas el valor del resto
        const auto& item {inventory.at(index)};
        return (item.price * item.quantity) + inventoryValueRec(inventory, index + 1);
    }
} // namespace CyberPunkCba
