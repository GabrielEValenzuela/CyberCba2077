#include "shopCommand.hpp"
#include "common/types.hpp"
#include "model/gameModel.hpp"

#include <iostream>

namespace CyberpunkCba
{

    ShopCommand::ShopCommand()
    {
        m_catalog = {{"Ciberimplante de brazo", ItemType::Tech, 500, 1},
                     {"Ciberimplante de pierna", ItemType::Tech, 300, 1},
                     {"Ciberimplante ocular", ItemType::Tech, 400, 1},
                     {"Ciberimplante neural", ItemType::Tech, 800, 1},
                     {"Pistola inteligente", ItemType::Weapon, 350, 1},
                     {"Rifle de asalto", ItemType::Weapon, 600, 1},
                     {"Granada de fragmentación", ItemType::Weapon, 150, 1},
                     {"Kit de primeros auxilios", ItemType::Consumable, 100, 1},
                     {"Software de hackeo avanzado", ItemType::Tech, 450, 1}};
    }

    void ShopCommand::execute(GameModel& model)
    {
        // Lógica para mostrar el catálogo de la tienda y permitir al jugador comprar items
        // std::cout << "Bienvenido a la tienda. Aquí está nuestro catálogo:\n";
        const int credits = model.credits();
        int asequibles = 0;

        // Calcular cuantos items son asequibles antes de imprimir
        for (const auto& item : m_catalog)
        {
            if (isAffordable(item, credits))
            {
                asequibles++;
            }
        }

        // 2 - Inventario lleno
        if (model.isInventoryFull())
        {
            std::cout << "======================================================\n";
            std::cout << "Tu inventario está lleno. No puedes comprar más items.\n";
            std::cout << "======================================================\n";
            return;
        }
        // 3 - No tiene créditos para comprar nada
        else if (asequibles == 0)
        {
            std::cout << "========================================================\n";
            std::cout << "No tienes suficientes créditos para comprar ningún item.\n";
            std::cout << "========================================================\n";
            return;
        }

        // Estado del catalogo normal
        for (const auto& item : m_catalog)
        {
            if (isAffordable(item, credits))
            {
                std::cout << "(✓) ";
            }
            else
                std::cout << "(✗) ";
            std::cout << item.name << " (Precio: " << item.price << " créditos)\n";
        }
        std::cout << "Items asequibles: " << asequibles << '\n';
    }

    std::string ShopCommand::name() const
    {
        // Retorna el nombre del comando, que se usará para invocarlo
        return "shop";
    }

    std::string ShopCommand::description() const
    {
        // Retorna una descripción breve del comando para mostrar en el help
        return "Abre la tienda para comprar ítems.";
    }

    std::string ShopCommand::category() const
    {
        // Retorna la categoría del comando para agruparlo en el help
        return "mundo";
    }

    bool ShopCommand::isAffordable(const Item& item, int credits) const
    {
        // Verifica si el jugador tiene suficiente dinero para comprar el item
        return credits >= item.price;
    }
} // namespace CyberpunkCba