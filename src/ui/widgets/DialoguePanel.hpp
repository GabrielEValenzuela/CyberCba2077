#ifndef DIALOGUE_PANEL_HPP
#define DIALOGUE_PANEL_HPP

/**
 * @file DialoguePanel.hpp
 * @brief Reusable single-response dialogue line (speaker + text + a single
 * continue hint), distinct from ChoicePanel which renders multiple response
 * buttons. Used for lines that don't branch (transmissions, NPC beats).
 */
#include <string>

#include "raylib.h"

class DialoguePanel
{
public:
    /**
     * @param speaker Speaker name.
     * @param portrait Head-and-shoulders portrait (PortraitFrame), or nullptr
     * to render text-only (falls back to the pre-ADR-0017 layout).
     * @param accent Portrait frame border / speaker name color.
     * @param body Dialogue line; wrapped internally.
     * @param hint Continue prompt (e.g. "ENTER: continuar").
     * @param font UI font, or nullptr to fall back to raylib's default.
     */
    static void draw(Rectangle bounds, const std::string& speaker, const Texture2D* portrait, Color accent,
                      const std::string& body, const std::string& hint, const Font* font);
};

#endif // DIALOGUE_PANEL_HPP
