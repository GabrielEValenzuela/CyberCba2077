#include "ui/widgets/StatusPanel.hpp"

#include "ui/NlmColors.hpp"

void StatusPanel::draw(Rectangle bounds, const std::vector<cybercba::StatusEffect>& statuses, const Font* font)
{
    if (statuses.empty()) return;
    float x = bounds.x;
    const float chipHeight = bounds.height;
    for (const auto& status : statuses)
    {
        const std::string label = status.id + " (" + std::to_string(status.remainingTurns) + ")";
        const int textSize = 13;
        const float textWidth = font != nullptr ? MeasureTextEx(*font, label.c_str(), static_cast<float>(textSize), 1.0F).x
                                                 : static_cast<float>(MeasureText(label.c_str(), textSize));
        const float chipWidth = textWidth + 16.0F;
        if (x + chipWidth > bounds.x + bounds.width) break;
        const Rectangle chip {x, bounds.y, chipWidth, chipHeight};
        DrawRectangleRec(chip, NlmColors::BG_CARD);
        DrawRectangleLinesEx(chip, 1.0F, NlmColors::AMBER);
        const float textY = bounds.y + (chipHeight - static_cast<float>(textSize)) / 2.0F;
        if (font != nullptr)
        {
            DrawTextEx(*font, label.c_str(), {x + 8.0F, textY}, static_cast<float>(textSize), 1.0F, NlmColors::AMBER);
        }
        else
        {
            DrawText(label.c_str(), static_cast<int>(x + 8.0F), static_cast<int>(textY), textSize, NlmColors::AMBER);
        }
        x += chipWidth + 6.0F;
    }
}
