#ifndef SHOP_COMMAND_HPP
#define SHOP_COMMAND_HPP
//evita inclusiones múltiples

#include "controller/command.hpp" // incluyo clase base Command, para heredar
#include "common/types.hpp" 

#include <string>
#include <vector>


/**
* @file shopCommand.hpp
* @brief Header privado de ShopCommand.
*
* @details
* Este header NO es parte de la API pública del sistema.
* Solo debe ser incluido por ShopCommand.cpp.
* La API pública es Command.hpp.
*
* @author Equipo LCS Y MJ — Exodus Systems Inc.
* @version 0.1.0
*/

namespace CyberpunkCba {
    /**
     * @class ShopCommand
     * @brief Comando para interactuar con la tienda del juego.
     */
    class ShopCommand final : public Command { // final: no se puede heredar de esta clase
        public:
            ShopCommand(); // constructor por defecto
            ~ShopCommand() override = default; // destructor virtual, override: asegura que se sobreescribe el método de la clase base
             //implementacion de la interfaz command, son private porque solo se usan dentro de esta clase
            void execute(GameModel& model) override;
            [[nodiscard]]std::string name() const override;
            [[nodiscard]]std::string description() const override;
            [[nodiscard]]std::string category() const override;
            
        private:
             [[nodiscard]]bool isAffordable(const Item& item, int playerMoney)const; // función para verificar si el jugador puede comprar un item
             std::vector<Item> m_catalog; // catálogo de items disponibles en la tienda
    };
}
#endif // SHOP_COMMAND_HPP

