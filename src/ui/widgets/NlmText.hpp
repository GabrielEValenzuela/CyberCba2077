#ifndef NLM_TEXT_HPP
#define NLM_TEXT_HPP

/**
 * @file NlmText.hpp
 * @brief Utilidades de texto que respetan la paleta Cyber CBA.
 * @details Centraliza el uso de la fuente por defecto de raylib.
 */
#include "raylib.h"

class NlmText
{
public:
    /** @brief Dibuja texto con la fuente por defecto. @param text Texto. @param x Coordenada horizontal. @param y
     * Coordenada vertical. @param size Tamaño. @param color Color. */
    static void draw(const char* text, int x, int y, int size, Color color);
    /** @brief Centra texto horizontalmente. @param text Texto. @param y Coordenada vertical. @param size Tamaño. @param
     * color Color. */
    static void drawCentered(const char* text, int y, int size, Color color);
};

#endif // NLM_TEXT_HPP
