#ifndef NLM_PANEL_HPP
#define NLM_PANEL_HPP

/**
 * @file NlmPanel.hpp
 * @brief Panel de fondo oscuro con borde CyberCba2077.
 * @details Widget de presentación sin estado ni dependencia del modelo.
 */
#include "raylib.h"

class NlmPanel
{
public:
    /** @brief Dibuja un panel. @param bounds Área del panel. @param border Color del borde. */
    static void draw(Rectangle bounds, Color border);
};

#endif // NLM_PANEL_HPP
