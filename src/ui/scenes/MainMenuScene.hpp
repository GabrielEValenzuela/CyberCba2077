#ifndef NLM_MAIN_MENU_SCENE_HPP
#define NLM_MAIN_MENU_SCENE_HPP

/**
 * @file MainMenuScene.hpp
 * @brief Menú principal navegable por teclado.
 * @details Ofrece nueva partida, carga de partida y salida sin conocer la lógica del juego.
 */
#include "ui/IScene.hpp"

class MainMenuScene final : public IScene
{
public:
    /** @brief Lee navegación de menú. @param dt Delta time no utilizado. @post Puede solicitar mapa o salida. */
    void update(float dt) override;
    /** @brief Dibuja las tres opciones principales. */
    void draw() const override;
    /** @brief Consulta si el usuario eligió una opción. @return `true` cuando hay transición. */
    bool shouldTransition() const override;
    /** @brief Devuelve el destino solicitado. @return ID de mapa o salida. */
    int nextScene() const override;

private:
    int m_selectedIndex {0};
    bool m_shouldTransition {false};
    int m_nextScene {1};
};

#endif // NLM_MAIN_MENU_SCENE_HPP
