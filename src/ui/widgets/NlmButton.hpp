#ifndef NLM_BUTTON_HPP
#define NLM_BUTTON_HPP

/**
 * @file NlmButton.hpp
 * @brief Botón navegable con estilo CyberCba2077.
 * @details El foco es controlado externamente por la escena que lo utiliza.
 */
#include "raylib.h"

class NlmButton
{
public:
    /** @brief Construye un botón. @param bounds Área clickeable. @param label Texto mostrado. */
    NlmButton(Rectangle bounds, const char* label);
    /** @brief Dibuja el botón. @param focused Indica si tiene foco de teclado. */
    void draw(bool focused) const;
    /** @brief Comprueba si el mouse está dentro. @return `true` si el mouse está sobre el botón. */
    bool isHovered() const;

private:
    Rectangle m_bounds;
    const char* m_label;
};

#endif // NLM_BUTTON_HPP
