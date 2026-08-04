#include "ui/widgets/CurrentObjectivePanel.hpp"

#include "ui/NlmColors.hpp"
#include "ui/widgets/TextWrap.hpp"

void CurrentObjectivePanel::draw(Rectangle bounds, const std::string& title, const std::string& body, const Font* font)
{
    const int titleSize = 16;
    if (font != nullptr)
    {
        DrawTextEx(*font, title.c_str(), {bounds.x, bounds.y}, static_cast<float>(titleSize), 1.0F, NlmColors::AMBER);
    }
    else
    {
        DrawText(title.c_str(), static_cast<int>(bounds.x), static_cast<int>(bounds.y), titleSize, NlmColors::AMBER);
    }

    const int bodySize = 14;
    float y = bounds.y + static_cast<float>(titleSize) + 6.0F;
    for (const std::string& line : wrapText(body, static_cast<int>(bounds.width), bodySize, font))
    {
        if (y + static_cast<float>(bodySize) > bounds.y + bounds.height) break;
        if (font != nullptr)
        {
            DrawTextEx(*font, line.c_str(), {bounds.x, y}, static_cast<float>(bodySize), 1.0F, NlmColors::TEXT);
        }
        else
        {
            DrawText(line.c_str(), static_cast<int>(bounds.x), static_cast<int>(y), bodySize, NlmColors::TEXT);
        }
        y += static_cast<float>(bodySize) + 4.0F;
    }
}
