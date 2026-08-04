#include "ui/widgets/ChoicePanel.hpp"

#include "ui/NlmColors.hpp"
#include "ui/widgets/NlmPanel.hpp"
#include "ui/widgets/TextWrap.hpp"

void ChoicePanel::draw(Rectangle bounds, const std::string& speaker, const std::string& body,
                        const std::vector<std::string>& choices, int selectedIndex, const Font* font)
{
    NlmPanel::draw(bounds, NlmColors::CYAN);
    const float pad = 16.0F;
    if (font != nullptr)
    {
        DrawTextEx(*font, speaker.c_str(), {bounds.x + pad, bounds.y + 10.0F}, 20.0F, 1.0F, NlmColors::AMBER);
    }
    else
    {
        DrawText(speaker.c_str(), static_cast<int>(bounds.x + pad), static_cast<int>(bounds.y) + 10, 20, NlmColors::AMBER);
    }

    float y = bounds.y + 44.0F;
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
        y += 24.0F;
    }

    if (choices.empty()) return;
    const float rowY = bounds.y + bounds.height - 52.0F;
    const float gap = 16.0F;
    const float buttonWidth = (bodyWidth - gap * static_cast<float>(choices.size() - 1)) / static_cast<float>(choices.size());
    float x = bounds.x + pad;
    for (std::size_t i = 0; i < choices.size(); ++i)
    {
        const bool focused = static_cast<int>(i) == selectedIndex;
        const Rectangle buttonBounds {x, rowY, buttonWidth, 36.0F};
        NlmPanel::draw(buttonBounds, focused ? NlmColors::CYAN : NlmColors::TEXT_DIM);
        const int textSize = 16;
        const Color textColor = focused ? NlmColors::TEXT : NlmColors::TEXT_DIM;
        if (font != nullptr)
        {
            const Vector2 measured = MeasureTextEx(*font, choices[i].c_str(), static_cast<float>(textSize), 1.0F);
            DrawTextEx(*font, choices[i].c_str(), {x + (buttonWidth - measured.x) / 2.0F, rowY + (36.0F - measured.y) / 2.0F}, static_cast<float>(textSize), 1.0F, textColor);
        }
        else
        {
            const int textX = static_cast<int>(x + (buttonWidth - static_cast<float>(MeasureText(choices[i].c_str(), textSize))) / 2.0F);
            DrawText(choices[i].c_str(), textX, static_cast<int>(rowY) + 10, textSize, textColor);
        }
        x += buttonWidth + gap;
    }
}
