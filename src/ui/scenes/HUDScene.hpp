#ifndef NLM_HUD_SCENE_HPP
#define NLM_HUD_SCENE_HPP

/**
 * @file HUDScene.hpp
 * @brief Superposición de estado del jugador sobre el mapa.
 * @details Solo consulta GameModel y no participa de la navegación principal.
 */
#include "model/GameModel.hpp"
#include "ui/IScene.hpp"

class HUDScene final : public IScene
{
public:
    /** @brief Construye el HUD observando el modelo. @param model Modelo de solo lectura para la vista. */
    explicit HUDScene(const cyberpunk::GameModel& model);
    /** @brief No tiene estado temporal. @param dt Delta time ignorado. */
    void update(float dt) override;
    /** @brief Dibuja créditos y acción disponible. */
    void draw() const override;
    /** @brief El HUD no solicita transiciones. @return Siempre `false`. */
    bool shouldTransition() const override;
    /** @brief No tiene destino. @return ID actual del mapa. */
    int nextScene() const override;

private:
    const cyberpunk::GameModel& m_model;
};

#endif // NLM_HUD_SCENE_HPP
