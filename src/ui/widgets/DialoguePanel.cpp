#include "ui/widgets/DialoguePanel.hpp"

#include "ui/NlmColors.hpp"
#include "ui/widgets/NlmPanel.hpp"
#include "ui/widgets/PortraitFrame.hpp"
#include "ui/widgets/TextWrap.hpp"

void DialoguePanel::draw(Rectangle bounds, const std::string& speaker, const Texture2D* portrait, Color accent,
                          const std::string& body, const std::string& hint, const Font* font)
{
    NlmPanel::draw(bounds, accent);
    const float pad = 16.0F;
    const bool hasPortrait = portrait != nullptr && portrait->id != 0;
    float textX = bounds.x + pad;
    if (hasPortrait)
    {
        const float portraitSize = bounds.height - pad * 2.0F;
        PortraitFrame::draw({bounds.x + pad, bounds.y + pad, portraitSize, portraitSize}, portrait, accent);
        textX = bounds.x + pad + portraitSize + pad;
    }

    if (font != nullptr)
    {
        DrawTextEx(*font, speaker.c_str(), {textX, bounds.y + 12.0F}, 20.0F, 1.0F, NlmColors::AMBER);
    }
    else
    {
        DrawText(speaker.c_str(), static_cast<int>(textX), static_cast<int>(bounds.y) + 12, 20, NlmColors::AMBER);
    }

    float y = bounds.y + 46.0F;
    const float bodyWidth = bounds.x + bounds.width - pad - textX;
    for (const std::string& line : wrapText(body, static_cast<int>(bodyWidth), 19, font))
    {
        if (font != nullptr)
        {
            DrawTextEx(*font, line.c_str(), {textX, y}, 19.0F, 1.0F, NlmColors::TEXT);
        }
        else
        {
            DrawText(line.c_str(), static_cast<int>(textX), static_cast<int>(y), 19, NlmColors::TEXT);
        }
        y += 26.0F;
    }

    if (font != nullptr)
    {
        DrawTextEx(*font, hint.c_str(), {textX, bounds.y + bounds.height - 34.0F}, 18.0F, 1.0F, NlmColors::AMBER);
    }
    else
    {
        DrawText(hint.c_str(), static_cast<int>(textX), static_cast<int>(bounds.y + bounds.height - 34.0F), 18, NlmColors::AMBER);
    }
}
