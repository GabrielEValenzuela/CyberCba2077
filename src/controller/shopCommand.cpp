#include "shopCommand.hpp"
#include "model/gameModel.hpp"
#include "common/types.hpp"


#include <iostream>


namespace CyberpunkCba
{
    class ShopCommand final : public Command{ // final: no se puede heredar de esta clase
        public:
              //implementacion de la interfaz command, son private porque solo se usan dentro de esta clase
            void execute(GameModel& model) override;
            std::string name() const override;
            std::string description() const override;
            std::string category() const override;
        private:
            const bool isAffordable(const Item& item, int playerMoney); // función para verificar si el jugador puede comprar un item
            std::vector<Item> m_catalog; // catálogo de items disponibles en la tienda
    };
    
    ShopCommand::ShopCommand(){
        m_catalog = { {"Ciberimplante de brazo", ItemType::Tech, 500, 1},
                      {"Ciberimplante de pierna", ItemType::Tech, 300, 1},
                      {"Ciberimplante ocular", ItemType::Tech, 400, 1},
                      {"Ciberimplante neural", ItemType::Tech, 800, 1},
                      {"Pistola inteligente", ItemType::Weapon, 350, 1},
                      {"Rifle de asalto", ItemType::Weapon, 600, 1},
                      {"Granada de fragmentación", ItemType::Weapon, 150, 1},
                      {"Kit de primeros auxilios", ItemType::Consumable, 100, 1},
                      {"Software de hackeo avanzado", ItemType::Tech, 450, 1}
        };
    }

    void ShopCommand::execute(GameModel& model){
        // Lógica para mostrar el catálogo de la tienda y permitir al jugador comprar items
        std::cout << "Bienvenido a la tienda. Aquí está nuestro catálogo:\n";
        int asequibles = 0;
        // 1 - Mostrar catálogo con indicación de qué items puede comprar el jugador
        for (const auto& item : m_catalog) {
            if(isAffordable(item, model.credits())){
                std::cout << "(✓) ";
                asequibles++;
            } 
            else std::cout << "(✗) ";
            std::cout << item.name << " (Precio: " << item.price << " créditos)\n";
        }
        // 2 - Inventario lleno
        if(model.isInventoryFull()){
            std::cout << "======================================================\n";
            std::cout << "Tu inventario está lleno. No puedes comprar más items.\n";
            std::cout << "======================================================\n";    
            return;
        }
        // 3 - No tiene créditos para comprar nada
        else if(asequibles == 0){
            std::cout << "========================================================\n";
            std::cout << "No tienes suficientes créditos para comprar ningún item.\n";
            std::cout << "========================================================\n";
            return;
        }
        
    }

    std::string ShopCommand::name() const{
        // Retorna el nombre del comando, que se usará para invocarlo
        return "shop";
    }

    std::string ShopCommand::description() const{
        // Retorna una descripción breve del comando para mostrar en el help
        return "Abre la tienda para comprar ítems.";
    }

    std::string ShopCommand::category() const{
        // Retorna la categoría del comando para agruparlo en el help
        return "mundo";
    }

    const bool ShopCommand::isAffordable(const Item& item, int playerMoney){
            // Verifica si el jugador tiene suficiente dinero para comprar el item
        return playerMoney >= item.price;
    }


    
    






} // namespace CyberpunkCba