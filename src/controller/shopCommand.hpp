#ifndef _SHOP_COMMAND_HPP
#define _SHOP_COMMAND_HPP
// evita inclusiones múltiples

#include "common/types.hpp"
#include "controller/command.hpp" // incluyo clase base Command, para heredar

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

namespace CyberpunkCba
{
    /**
     * @class ShopCommand
     * @brief Comando para interactuar con la tienda del juego.
     */
    class ShopCommand final : public Command
    {
    public:
        ShopCommand(); // constructor por defecto
        ~ShopCommand() override = default;

    private:
        void execute(GameModel& model) override;
        [[nodiscard]] std::string name() const override;
        [[nodiscard]] std::string description() const override;
        [[nodiscard]] std::string category() const override;

        [[nodiscard]] bool isAffordable(const Item& item, int playerMoney) const; // función para verificar si el jugador puede comprar un item

        std::vector<Item> m_catalog; // catálogo de items disponibles en la tienda
    };

} // namespace CyberpunkCba

#endif // _SHOP_COMMAND_HPP
