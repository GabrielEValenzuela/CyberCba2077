#ifndef NLM_ISCENE_HPP
#define NLM_ISCENE_HPP

/**
 * @file IScene.hpp
 * @brief Interfaz base para todas las escenas del juego.
 * @details Toda escena implementa actualización y renderizado; SceneManager es dueño del loop de escenas.
 * @invariant Una escena no dibuja fuera de BeginDrawing/EndDrawing.
 */
class IScene
{
public:
    virtual ~IScene() = default;
    /** @brief Procesa input y actualiza estado. @param dt Delta time en segundos. */
    virtual void update(float dt) = 0;
    /** @brief Renderiza la escena. @pre Debe llamarse entre BeginDrawing y EndDrawing. */
    virtual void draw() const = 0;
    /** @brief Indica si la escena solicita una transición. @return `true` si hay transición pendiente. */
    virtual bool shouldTransition() const = 0;
    /** @brief Devuelve el identificador de destino. @return ID válido cuando `shouldTransition()` es `true`. */
    virtual int nextScene() const = 0;
};

#endif // NLM_ISCENE_HPP
