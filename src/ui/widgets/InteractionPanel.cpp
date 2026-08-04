#include "ui/widgets/InteractionPanel.hpp"

#include <algorithm>

#include "ui/NlmColors.hpp"
#include "ui/widgets/ActionMenu.hpp"
#include "ui/widgets/NlmPanel.hpp"
#include "ui/widgets/PortraitFrame.hpp"
#include "ui/widgets/TextWrap.hpp"

void InteractionPanel::draw(Rectangle bounds, const std::string& label, const Texture2D* portrait, Color accent,
                             const std::string& body, const std::vector<std::string>& actions, int selectedAction, const Font* font)
{
    NlmPanel::draw(bounds, accent);
    const float pad = 12.0F;
    const int labelSize = 13;
    const float labelReserve = static_cast<float>(labelSize) + 6.0F;
    const float portraitSize = bounds.height - pad * 2.0F - labelReserve;
    const Rectangle portraitBounds {bounds.x + pad, bounds.y + pad, portraitSize, portraitSize};
    PortraitFrame::draw(portraitBounds, portrait, accent);

    if (font != nullptr)
    {
        DrawTextEx(*font, label.c_str(), {portraitBounds.x, portraitBounds.y + portraitSize + 6.0F}, static_cast<float>(labelSize), 1.0F, accent);
    }
    else
    {
        DrawText(label.c_str(), static_cast<int>(portraitBounds.x), static_cast<int>(portraitBounds.y + portraitSize + 6.0F), labelSize, accent);
    }

    const float actionMenuWidth = actions.empty() ? 0.0F : 280.0F;
    const float textX = portraitBounds.x + portraitSize + pad * 1.5F;
    const float textWidth = bounds.width - (textX - bounds.x) - actionMenuWidth - pad * 2.0F;
    float y = bounds.y + pad;
    const int bodySize = 17;
    for (const std::string& line : wrapText(body, static_cast<int>(textWidth), bodySize, font))
    {
        if (y + static_cast<float>(bodySize) > bounds.y + bounds.height - pad) break;
        if (font != nullptr)
        {
            DrawTextEx(*font, line.c_str(), {textX, y}, static_cast<float>(bodySize), 1.0F, NlmColors::TEXT);
        }
        else
        {
            DrawText(line.c_str(), static_cast<int>(textX), static_cast<int>(y), bodySize, NlmColors::TEXT);
        }
        y += static_cast<float>(bodySize) + 8.0F;
    }

    if (!actions.empty())
    {
        // Item height/gap shrink to fit however many actions this node has —
        // a fixed 42px item overflowed the panel past 4 actions (see
        // docs/mission-screen-templates.md pivot notes).
        const float availableHeight = bounds.height - pad * 2.0F;
        const float gap = 6.0F;
        const int count = static_cast<int>(actions.size());
        const float itemHeight = std::clamp((availableHeight - gap * static_cast<float>(count - 1)) / static_cast<float>(count), 22.0F, 40.0F);
        const Rectangle menuBounds {bounds.x + bounds.width - actionMenuWidth - pad, bounds.y + pad, actionMenuWidth, 0};
        ActionMenu::draw(menuBounds, actions, selectedAction, font, itemHeight, gap);
    }
}
