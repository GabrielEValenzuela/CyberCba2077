#ifndef TRANSMISSION_PANEL_HPP
#define TRANSMISSION_PANEL_HPP

/**
 * @file TransmissionPanel.hpp
 * @brief Reusable "fragmented transmission" panel: signal-quality bar +
 * procedural noise lines (allowed effect per the minimal-animation policy —
 * no sprite sheets, just DrawLineEx noise) + body text. Used for the
 * prologue's fragmented_transmission node and similar broken-signal beats.
 */
#include <string>

#include "raylib.h"

class TransmissionPanel
{
public:
    /**
     * @param signalQuality 0..1; low values draw denser noise and a redder bar.
     * @param elapsedSeconds Drives the noise animation; pass a monotonically
     * increasing time source (e.g. GetTime()) so the effect isn't static.
     */
    static void draw(Rectangle bounds, const std::string& label, const std::string& body, float signalQuality, float elapsedSeconds, const Font* font);
};

#endif // TRANSMISSION_PANEL_HPP
