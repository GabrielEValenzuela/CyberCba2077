#include "ui/widgets/EvidencePanel.hpp"

#include "ui/NlmColors.hpp"
#include "ui/widgets/NlmPanel.hpp"

void EvidencePanel::draw(Rectangle bounds, const Texture2D* image, const std::string& title, const Font* font)
{
    NlmPanel::draw(bounds, NlmColors::CYAN);
    const float thumbSize = bounds.height - 12.0F;
    const Rectangle thumb {bounds.x + 6.0F, bounds.y + 6.0F, thumbSize, thumbSize};
    if (image != nullptr && image->id != 0)
    {
        const Rectangle src {0, 0, static_cast<float>(image->width), static_cast<float>(image->height)};
        DrawTexturePro(*image, src, thumb, {0, 0}, 0.0F, WHITE);
    }
    else
    {
        DrawRectangleLinesEx(thumb, 1.0F, NlmColors::TEXT_DIM);
    }
    const int textX = static_cast<int>(thumb.x + thumb.width + 8.0F);
    const int textY = static_cast<int>(bounds.y + 8.0F);
    const int size = 15;
    if (font != nullptr)
    {
        DrawTextEx(*font, title.c_str(), {static_cast<float>(textX), static_cast<float>(textY)}, static_cast<float>(size), 1.0F, NlmColors::TEXT);
    }
    else
    {
        DrawText(title.c_str(), textX, textY, size, NlmColors::TEXT);
    }
}
