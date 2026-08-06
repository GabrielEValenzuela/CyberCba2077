#include "ui/widgets/MissionResultsView.hpp"

#include "ui/NlmColors.hpp"
#include "ui/widgets/EvidencePanel.hpp"
#include "ui/widgets/NlmPanel.hpp"
#include "ui/widgets/TextWrap.hpp"

void MissionResultsView::draw(Rectangle bounds, const std::string& title, const std::string& body, const std::string& hint,
                               const std::vector<EvidenceEntry>& evidence, const Font* font)
{
    NlmPanel::draw(bounds, NlmColors::CYAN);
    const float pad = 24.0F;
    const int titleSize = 34;
    if (font != nullptr)
    {
        DrawTextEx(*font, title.c_str(), {bounds.x + pad, bounds.y + 20.0F}, static_cast<float>(titleSize), 1.0F, NlmColors::CYAN);
    }
    else
    {
        DrawText(title.c_str(), static_cast<int>(bounds.x + pad), static_cast<int>(bounds.y) + 20, titleSize, NlmColors::CYAN);
    }

    float y = bounds.y + 20.0F + static_cast<float>(titleSize) + 20.0F;
    const float bodyWidth = bounds.width - pad * 2.0F;
    const float evidenceStripHeight = evidence.empty() ? 0.0F : 100.0F;
    const float bodyBottom = bounds.y + bounds.height - 50.0F - evidenceStripHeight;
    for (const std::string& line : wrapText(body, static_cast<int>(bodyWidth), 19, font))
    {
        if (y + 24.0F > bodyBottom) break;
        if (font != nullptr)
        {
            DrawTextEx(*font, line.c_str(), {bounds.x + pad, y}, 19.0F, 1.0F, NlmColors::TEXT);
        }
        else
        {
            DrawText(line.c_str(), static_cast<int>(bounds.x + pad), static_cast<int>(y), 19, NlmColors::TEXT);
        }
        y += 26.0F;
    }

    if (!evidence.empty())
    {
        const float stripY = bounds.y + bounds.height - 50.0F - evidenceStripHeight + 10.0F;
        const int labelSize = 18;
        if (font != nullptr)
        {
            DrawTextEx(*font, "EVIDENCIAS RECUPERADAS", {bounds.x + pad, stripY}, static_cast<float>(labelSize), 1.0F, NlmColors::CYAN);
        }
        else
        {
            DrawText("EVIDENCIAS RECUPERADAS", static_cast<int>(bounds.x + pad), static_cast<int>(stripY), labelSize, NlmColors::CYAN);
        }
        float x = bounds.x + pad;
        const float chipWidth = 300.0F, chipHeight = 60.0F, chipGap = 12.0F;
        const float chipY = stripY + static_cast<float>(labelSize) + 8.0F;
        for (const auto& item : evidence)
        {
            if (x + chipWidth > bounds.x + bounds.width - pad) break;
            EvidencePanel::draw({x, chipY, chipWidth, chipHeight}, item.image, item.title, font);
            x += chipWidth + chipGap;
        }
    }

    const int hintSize = 18;
    if (font != nullptr)
    {
        DrawTextEx(*font, hint.c_str(), {bounds.x + pad, bounds.y + bounds.height - 34.0F}, static_cast<float>(hintSize), 1.0F, NlmColors::AMBER);
    }
    else
    {
        DrawText(hint.c_str(), static_cast<int>(bounds.x + pad), static_cast<int>(bounds.y + bounds.height - 34.0F), hintSize, NlmColors::AMBER);
    }
}
