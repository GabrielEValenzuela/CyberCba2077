#ifndef NLM_MAP_SCENE_HPP
#define NLM_MAP_SCENE_HPP

/**
 * @file MapScene.hpp
 * @brief Vista del mapa de Cyber CBA.
 * @details Dibuja una topología temporal hasta que el modelo exponga un grafo de estaciones.
 */
#include "ui/IScene.hpp"

class MapScene final : public IScene
{
public:
    /** @brief Procesa apertura de inventario o regreso a menú. @param dt Delta time no utilizado. @post Puede solicitar
     * transición. */
    void update(float dt) override;
    /** @brief Dibuja nodos y aristas de la topología temporal. */
    void draw() const override;
    /** @brief Consulta solicitud de transición. @return `true` si hay destino pendiente. */
    bool shouldTransition() const override;
    /** @brief Devuelve destino de la transición. @return ID de inventario o menú. */
    int nextScene() const override;

private:
    bool m_shouldTransition {false};
    int m_nextScene {2};
};

#endif // NLM_MAP_SCENE_HPP
