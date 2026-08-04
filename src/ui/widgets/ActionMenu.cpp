#include "ui/widgets/ActionMenu.hpp"

#include <algorithm>

#include "ui/NlmColors.hpp"

namespace
{
// Matches the CARD constant previously local to src/app/GameApp.cpp so
// GameApp::drawButton's visual output is unchanged after delegating here.
constexpr Color CARD {5, 25, 40, 238};
}

void ActionMenu::drawItem(Rectangle bounds, const std::string& label, bool selected, bool disabled, const Font* font)
{
    const int x = static_cast<int>(bounds.x), y = static_cast<int>(bounds.y), width = static_cast<int>(bounds.width), height = static_cast<int>(bounds.height);
    const Color border = disabled ? Color {48, 70, 80, 255} : selected ? NlmColors::CYAN : Color {48, 94, 110, 255};
    DrawRectangle(x, y, width, height, CARD);
    DrawRectangleLinesEx(bounds, selected ? 3.0F : 1.0F, border);
    const int markerHeight = std::min(24, height - 8);
    if (selected) DrawRectangle(x + 10, y + (height - markerHeight) / 2, 5, markerHeight, NlmColors::AMBER);
    const Color textColor = disabled ? NlmColors::TEXT_DIM : NlmColors::TEXT;
    const int textY = y + (height - 21) / 2;
    if (font != nullptr)
    {
        DrawTextEx(*font, label.c_str(), {static_cast<float>(x + 32), static_cast<float>(textY)}, 21.0F, 1.0F, textColor);
    }
    else
    {
        DrawText(label.c_str(), x + 32, textY, 21, textColor);
    }
}

void ActionMenu::draw(Rectangle bounds, const std::vector<std::string>& labels, int selectedIndex, const Font* font, float itemHeight, float gap)
{
    for (std::size_t i = 0; i < labels.size(); ++i)
    {
        const Rectangle itemBounds {bounds.x, bounds.y + static_cast<float>(i) * (itemHeight + gap), bounds.width, itemHeight};
        drawItem(itemBounds, labels[i], static_cast<int>(i) == selectedIndex, false, font);
    }
}
