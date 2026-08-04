#ifndef STORY_PANEL_HPP
#define STORY_PANEL_HPP

/**
 * @file StoryPanel.hpp
 * @brief Reusable "story / prologue" screen template (chapter label +
 * optional portrait/evidence image + wrapped story text). See
 * docs/mission-screen-templates.md template 6.1.
 * @details Presentation only; the caller owns text content and image
 * lookup (AssetStore), so this widget stays reusable across intro
 * narration, memory scenes, and evidence-discovery beats alike.
 */
#include <string>

#include "raylib.h"

class StoryPanel
{
public:
    /**
     * @brief Draws a story card.
     * @param bounds Card area.
     * @param label Chapter/context label drawn at the top (e.g. "PROLOGO").
     * @param image Portrait/evidence illustration, or nullptr to render the
     * body text full-width (no fake placeholder art, per AGENTS.md sec.8).
     * @param body Story text; wrapped internally against the text column.
     * @param font UI font, or nullptr to fall back to raylib's default.
     * @param labelColor Accent color for the label (e.g. cyan for Emma/red
     * for Magga scenes, per docs/character-design.md).
     */
    static void draw(Rectangle bounds, const std::string& label, const Texture2D* image, const std::string& body, const Font* font, Color labelColor);
};

#endif // STORY_PANEL_HPP
