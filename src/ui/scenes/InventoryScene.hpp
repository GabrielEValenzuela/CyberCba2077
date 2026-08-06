#ifndef NLM_INVENTORY_SCENE_HPP
#define NLM_INVENTORY_SCENE_HPP

/**
 * @file InventoryScene.hpp
 * @brief Vista de inventario de solo lectura.
 * @details Muestra el único estado de inventario disponible en el modelo actual: créditos.
 */
#include "model/GameModel.hpp"
#include "ui/IScene.hpp"

class InventoryScene final : public IScene
{
public:
    /** @brief Construye la vista observando el modelo. @param model Modelo que se consulta sin modificar. */
    explicit InventoryScene(const cyberpunk::GameModel& model);
    /** @brief Procesa regreso al mapa. @param dt Delta time no utilizado. @post Puede solicitar el mapa. */
    void update(float dt) override;
    /** @brief Dibuja el contenido disponible. */
    void draw() const override;
    /** @brief Consulta transición de regreso. @return `true` tras presionar Escape o I. */
    bool shouldTransition() const override;
    /** @brief Devuelve el ID del mapa. @return ID de `MapScene`. */
    int nextScene() const override;

private:
    const cyberpunk::GameModel& m_model;
    bool m_shouldTransition {false};
};

#endif // NLM_INVENTORY_SCENE_HPP
