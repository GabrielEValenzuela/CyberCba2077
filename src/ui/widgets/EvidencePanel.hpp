#ifndef EVIDENCE_PANEL_HPP
#define EVIDENCE_PANEL_HPP

/**
 * @file EvidencePanel.hpp
 * @brief Reusable evidence-journal chip: thumbnail (when a processed asset
 * exists) plus title, following the "evidence journal" template. Presentation
 * only; discovery/gating state lives in cybercba::EvidenceJournal.
 */
#include <string>

#include "raylib.h"

class EvidencePanel
{
public:
    /**
     * @brief Draws one evidence chip.
     * @param bounds Chip area.
     * @param image Processed evidence texture, or nullptr to fall back to a
     * bordered placeholder rect (never a fake "finished art" rectangle drawn
     * as if it were the real asset — see AGENTS.md sec.8).
     * @param title Evidence title (already localized by the caller).
     * @param font UI font for the title, or nullptr to use raylib's default.
     */
    static void draw(Rectangle bounds, const Texture2D* image, const std::string& title, const Font* font);
};

#endif // EVIDENCE_PANEL_HPP
