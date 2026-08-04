#include "ui/widgets/StoryPanel.hpp"

#include <algorithm>

#include "ui/NlmColors.hpp"
#include "ui/widgets/NlmPanel.hpp"
#include "ui/widgets/TextWrap.hpp"

namespace
{
constexpr float lineGap = 10.0F;

// Vertically centers the wrapped block inside `availableHeight` instead of
// pinning it to the top — a short line or two inside a tall panel otherwise
// reads as sparse/hard to focus on (user feedback: "no se lee bien").
void drawWrappedCentered(const std::string& text, float x, float y, float maxWidth, float availableHeight, int fontSize, const Font* font, Color color)
{
    const auto lines = wrapText(text, static_cast<int>(maxWidth), fontSize, font);
    const float lineHeight = static_cast<float>(fontSize) + lineGap;
    const float blockHeight = static_cast<float>(lines.size()) * lineHeight;
    float lineY = y + std::max(0.0F, (availableHeight - blockHeight) / 2.0F);
    for (const std::string& line : lines)
    {
        if (font != nullptr)
        {
            DrawTextEx(*font, line.c_str(), {x, lineY}, static_cast<float>(fontSize), 1.0F, color);
        }
        else
        {
            DrawText(line.c_str(), static_cast<int>(x), static_cast<int>(lineY), fontSize, color);
        }
        lineY += lineHeight;
    }
}
} // namespace

void StoryPanel::draw(Rectangle bounds, const std::string& label, const Texture2D* image, const std::string& body, const Font* font, Color labelColor)
{
    NlmPanel::draw(bounds, labelColor);
    const float labelSize = 20.0F;
    if (font != nullptr)
    {
        DrawTextEx(*font, label.c_str(), {bounds.x + 16.0F, bounds.y + 12.0F}, labelSize, 1.0F, labelColor);
    }
    else
    {
        DrawText(label.c_str(), static_cast<int>(bounds.x) + 16, static_cast<int>(bounds.y) + 12, static_cast<int>(labelSize), labelColor);
    }

    const float contentY = bounds.y + 48.0F;
    const float contentHeight = bounds.height - 60.0F;
    const bool hasImage = image != nullptr && image->id != 0;
    const float imageWidth = hasImage ? bounds.height * 0.55F : 0.0F;

    if (hasImage)
    {
        const Rectangle dest {bounds.x + 16.0F, contentY, imageWidth, contentHeight};
        const Rectangle src {0, 0, static_cast<float>(image->width), static_cast<float>(image->height)};
        DrawTexturePro(*image, src, dest, {0, 0}, 0.0F, WHITE);
    }

    const float textX = bounds.x + 16.0F + imageWidth + (hasImage ? 16.0F : 0.0F);
    const float textWidth = bounds.width - 32.0F - imageWidth - (hasImage ? 16.0F : 0.0F);
    drawWrappedCentered(body, textX, contentY, textWidth, contentHeight, 21, font, NlmColors::TEXT);
}
