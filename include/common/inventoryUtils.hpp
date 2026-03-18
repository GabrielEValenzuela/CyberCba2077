//
// Created by gvalenzuela on 3/18/26.
//

#ifndef CYBERPUNK_CORDOBA_2077_INVENTORYUTILS_HPP
#define CYBERPUNK_CORDOBA_2077_INVENTORYUTILS_HPP
#include "types.hpp"

#include <cstddef>
#include <vector>

/**
 * @file inventoryUtils.hpp
 * @brief Clase de utilidad para recorrer el inventario de forma recursiva
 *
 * */

namespace CyberPunkCba
{
    /**
     * @brief Calcula el valor total del inventario en creditos usando recursividad
     *
     * @param inventory Vector de inventario de items del runner
     * @param index Indice del item actual.
     * @return Valor total en creditos. 0 si el inventario esta vacio
     */
    int inventoryValueRec(const std::vector<CyberpunkCba::Item>& inventory, const std::size_t index);
}

#endif // CYBERPUNK_CORDOBA_2077_INVENTORYUTILS_HPP
