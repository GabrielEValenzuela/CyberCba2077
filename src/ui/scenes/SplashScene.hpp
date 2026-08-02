#ifndef NLM_SPLASH_SCENE_HPP
#define NLM_SPLASH_SCENE_HPP

/**
 * @file SplashScene.hpp
 * @brief Escena inicial temporizada de Cyber CBA.
 * @details Muestra identidad visual durante el tiempo definido en configuración antes de abrir el menú.
 */
#include "ui/IScene.hpp"

class SplashScene final : public IScene
{
public:
    /** @brief Construye el splash. @param durationSeconds Duración configurada, no negativa. */
    explicit SplashScene(float durationSeconds);
    /** @brief Actualiza el contador de la escena. @param dt Delta time en segundos. @post Solicita menú al cumplir dos
     * segundos. */
    void update(float dt) override;
    /** @brief Dibuja el logo y título. */
    void draw() const override;
    /** @brief Consulta la transición automática. @return `true` tras dos segundos. */
    bool shouldTransition() const override;
    /** @brief Devuelve el ID del menú. @return ID de `MainMenuScene`. */
    int nextScene() const override;

private:
    float m_elapsedSeconds {0.0F};
    float m_durationSeconds;
};

#endif // NLM_SPLASH_SCENE_HPP
