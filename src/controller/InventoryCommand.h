#ifndef _INVENTORY_COMMAND_HPP
#define _INVENTORY_COMMAND_HPP

#include "controller/command.hpp"
#include "common/types.hpp"

#include <string>
#include <vector>

/**
 
@file InventoryCommand.hpp
@brief Header privado de InventoryCommand.*
@details
Este header NO es parte de la API pública del sistema.
Solo debe ser incluido por InventoryCommand.cpp.
La API pública es Command.hpp.*
@author Equipo 01 — Exodus Systems Inc.
@version 0.1.0*/

namespace CyberpunkCba
{

    /**
     
@class InventoryCommand
@brief Muestra el inventario completo del jugador.*
@details
Muestra los ítems disponibles en la mochila del jugador junto con
su cantidad, el valor total en créditos y una barra visual de
capacidad. Alerta al jugador si el inventario está vacío o lleno.*/
class InventoryCommand final : public Command{
public:
InventoryCommand() = default;
        ~InventoryCommand() override = default;

    private:
        // Wazuh convention: implementaciones de interfaz son private.

        /**
         
@brief Muestra el estado actual del inventario del jugador.
@details Si el inventario está vacío, muestra mensaje apropiado.
Complejidad: O(n) donde n = cantidad de ítems.
@param model No modificado por este comando.
*/
void execute(GameModel& model) override;std::string name() const override;std::string description() const override;std::string category() const override;

        /**
         
@brief Calcula el valor total de todos los ítems del inventario.
@details Usa double para evitar overflow en la multiplicación
price * quantity con valores grandes.
@param inventario Vector de ítems del jugador.
@return Valor total en créditos.
*/
double calcularValorTotal(const std::vector<Item>& inventario) const;

        /**
         
@brief Genera una barra de progreso visual en texto.
@details Si maximo <= 0 devuelve una barra vacía por defecto.
Ejemplo de salida: [######.........]
@param actual Cantidad actual de ítems en el inventario.
@param maximo Capacidad máxima del inventario.
@param ancho Ancho total de la barra en caracteres.
@return String con la barra de progreso formateada.
*/std::string renderBar(int actual, int maximo, int ancho) const;
};

} // namespace CyberpunkCba

#endif // _INVENTORY_COMMAND_HPP