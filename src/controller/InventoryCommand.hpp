#ifndef INVENTORY_COMMAND_HPP
#define INVENTORY_COMMAND_HPP

#include "controller/command.hpp"
#include "common/types.hpp"
#include "model/gameModel.hpp"

#include <cstddef>
#include <string>
#include <vector>

/**
 * @file InventoryCommand.hpp
 * @brief Header privado de InventoryCommand.
 *
 * @details
 * Este header NO es parte de la API pública del sistema.
 * Solo debe ser incluido por InventoryCommand.cpp.
 * La API pública es Command.hpp.
 *
 * @author Equipo 01 — Exodus Systems Inc.
 * @version 0.2.0
 */

namespace CyberpunkCba
{

/**
 * @class InventoryCommand
 * @brief Muestra el inventario completo del jugador.
 *
 * @details
 * Lee el inventario desde archivo, calcula el valor total de forma
 * recursiva con inventoryValue() y lo muestra por pantalla.
 * Si el archivo no existe o está vacío, muestra inventario vacío con
 * valor 0 sin crashear.
 */
class InventoryCommand final : public Command
{
public:
    InventoryCommand()           = default;
    ~InventoryCommand() override = default;

    /**
     * @brief Calcula el valor total del inventario de forma recursiva.
     *
     * @details
     * Patrón: acumulación recursiva.\n
     * Caso base: index == inventory.size() → retorna 0.\n
     * Caso recursivo: price * quantity del ítem actual +
     * inventoryValue(inventory, index + 1).\n
     * Ítems con price < 0 o quantity < 0 son ignorados.
     *
     * @param inventory Vector de ítems del inventario.
     * @param index     Índice actual de procesamiento.
     * @return Valor total acumulado en créditos. 0 si el vector está vacío.
     */
    static int inventoryValue(const std::vector<Item>& inventory,
                              std::size_t              index);


private:
    // Wazuh convention: implementaciones de interfaz son private.


    /**
     * @brief Muestra el estado actual del inventario del jugador.
     *
     * @details
     * Lee el inventario desde archivo y calcula su valor total.
     * Si el inventario está vacío, muestra mensaje apropiado.
     *
     * @param model GameModel del jugador actual.
     */
    void execute(GameModel& model) override;

    /// @brief Retorna "inventario".
    [[nodiscard]] std::string name() const override;

    /// @brief Retorna descripción del comando.
    [[nodiscard]] std::string description() const override;

    /// @brief Retorna "runner".
    [[nodiscard]] std::string category() const override;
};

} // namespace CyberpunkCba

#endif // INVENTORY_COMMAND_HPP