#include "ui/widgets/TransmissionPanel.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>

#include "ui/NlmColors.hpp"
#include "ui/widgets/NlmPanel.hpp"
#include "ui/widgets/TextWrap.hpp"

void TransmissionPanel::draw(Rectangle bounds, const std::string& label, const std::string& body, float signalQuality, float elapsedSeconds, const Font* font)
{
    NlmPanel::draw(bounds, NlmColors::CYAN);
    const float pad = 16.0F;
    const float quality = std::clamp(signalQuality, 0.0F, 1.0F);

    if (font != nullptr)
    {
        DrawTextEx(*font, label.c_str(), {bounds.x + pad, bounds.y + 12.0F}, 20.0F, 1.0F, NlmColors::CYAN);
    }
    else
    {
        DrawText(label.c_str(), static_cast<int>(bounds.x + pad), static_cast<int>(bounds.y) + 12, 20, NlmColors::CYAN);
    }

    // Signal-quality bar: green when strong, amber/red as it degrades.
    const Rectangle barBounds {bounds.x + pad, bounds.y + 40.0F, bounds.width - pad * 2.0F, 10.0F};
    DrawRectangleRec(barBounds, NlmColors::BG_CARD);
    const Color barColor = quality > 0.6F ? NlmColors::GREEN : quality > 0.3F ? NlmColors::AMBER : NlmColors::RED;
    DrawRectangleRec({barBounds.x, barBounds.y, barBounds.width * quality, barBounds.height}, barColor);
    DrawRectangleLinesEx(barBounds, 1.0F, NlmColors::TEXT_DIM);

    // Procedural noise lines (allowed effect, no sprite sheet): denser and
    // more erratic the weaker the signal.
    const int noiseLines = static_cast<int>((1.0F - quality) * 18.0F);
    for (int i = 0; i < noiseLines; ++i)
    {
        const float seed = static_cast<float>(i) * 37.13F + elapsedSeconds * 60.0F;
        const float y = barBounds.y + barBounds.height + 6.0F + std::fmod(std::abs(std::sin(seed)) * (bounds.height - 90.0F), bounds.height - 90.0F);
        const float x0 = bounds.x + pad + std::fmod(std::abs(std::cos(seed * 1.7F)) * (bounds.width - pad * 2.0F - 40.0F), bounds.width - pad * 2.0F - 40.0F);
        DrawLineEx({x0, y}, {x0 + 30.0F, y}, 1.0F, Color {static_cast<unsigned char>(NlmColors::CYAN.r), static_cast<unsigned char>(NlmColors::CYAN.g), static_cast<unsigned char>(NlmColors::CYAN.b), 90});
    }

    float y = barBounds.y + barBounds.height + 24.0F;
    const float bodyWidth = bounds.width - pad * 2.0F;
    for (const std::string& line : wrapText(body, static_cast<int>(bodyWidth), 18, font))
    {
        if (font != nullptr)
        {
            DrawTextEx(*font, line.c_str(), {bounds.x + pad, y}, 18.0F, 1.0F, NlmColors::TEXT);
        }
        else
        {
            DrawText(line.c_str(), static_cast<int>(bounds.x + pad), static_cast<int>(y), 18, NlmColors::TEXT);
        }
        y += 25.0F;
    }
}
