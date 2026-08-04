#include "ui/widgets/TurnOrderPanel.hpp"

#include "ui/NlmColors.hpp"

void TurnOrderPanel::draw(Rectangle bounds, const std::vector<Entry>& order, const std::string& activeId, const Font* font)
{
    if (order.empty()) return;
    const float slotWidth = bounds.width / static_cast<float>(order.size());
    const int textSize = 14;
    for (std::size_t i = 0; i < order.size(); ++i)
    {
        const bool active = order[i].id == activeId;
        const float x = bounds.x + slotWidth * static_cast<float>(i);
        const Color color = active ? NlmColors::AMBER : NlmColors::TEXT_DIM;
        if (active) DrawRectangleLinesEx({x + 2.0F, bounds.y, slotWidth - 4.0F, bounds.height}, 1.0F, color);
        const float textWidth = font != nullptr ? MeasureTextEx(*font, order[i].label.c_str(), static_cast<float>(textSize), 1.0F).x
                                                 : static_cast<float>(MeasureText(order[i].label.c_str(), textSize));
        const float textX = x + (slotWidth - textWidth) / 2.0F;
        const float textY = bounds.y + (bounds.height - static_cast<float>(textSize)) / 2.0F;
        if (font != nullptr)
        {
            DrawTextEx(*font, order[i].label.c_str(), {textX, textY}, static_cast<float>(textSize), 1.0F, color);
        }
        else
        {
            DrawText(order[i].label.c_str(), static_cast<int>(textX), static_cast<int>(textY), textSize, color);
        }
        if (i + 1 < order.size())
            DrawText(">", static_cast<int>(x + slotWidth - 10.0F), static_cast<int>(bounds.y + bounds.height / 2.0F - 7.0F), textSize, NlmColors::TEXT_DIM);
    }
}
