#include "ui/widgets/EnemyIntentPanel.hpp"

#include "ui/NlmColors.hpp"
#include "ui/widgets/NlmPanel.hpp"

void EnemyIntentPanel::draw(Rectangle bounds, const std::string& intentLabel, const Font* font)
{
    NlmPanel::draw(bounds, NlmColors::AMBER);
    const std::string text = "INTENCION: " + intentLabel;
    const int textSize = 14;
    const float textY = bounds.y + (bounds.height - static_cast<float>(textSize)) / 2.0F;
    if (font != nullptr)
    {
        DrawTextEx(*font, text.c_str(), {bounds.x + 8.0F, textY}, static_cast<float>(textSize), 1.0F, NlmColors::AMBER);
    }
    else
    {
        DrawText(text.c_str(), static_cast<int>(bounds.x + 8.0F), static_cast<int>(textY), textSize, NlmColors::AMBER);
    }
}
