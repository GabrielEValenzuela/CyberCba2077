#include "ui/widgets/NodeMapView.hpp"

#include <algorithm>
#include <unordered_map>

#include "ui/NlmColors.hpp"

namespace
{
Color stateColor(const NodeMapView::Entry& entry)
{
    if (entry.current) return NlmColors::AMBER;
    if (entry.locked) return NlmColors::TEXT_DIM;
    if (entry.completed) return NlmColors::GREEN;
    return NlmColors::CYAN;
}

float measureWidth(const std::string& text, int fontSize, const Font* font)
{
    return font != nullptr ? MeasureTextEx(*font, text.c_str(), static_cast<float>(fontSize), 1.0F).x
                            : static_cast<float>(MeasureText(text.c_str(), fontSize));
}

// Truncates with an ASCII ellipsis (the loaded font codepoint set only
// covers ASCII + the Spanish accented range — see AssetStore::font — so a
// unicode "…" renders as tofu) so a long node name never spills into its
// neighbor's slot.
std::string fitToWidth(const std::string& text, float maxWidth, int fontSize, const Font* font)
{
    if (measureWidth(text, fontSize, font) <= maxWidth) return text;
    std::string truncated = text;
    while (!truncated.empty() && measureWidth(truncated + "...", fontSize, font) > maxWidth)
        truncated.pop_back();
    return truncated.empty() ? text.substr(0, 1) + "..." : truncated + "...";
}
} // namespace

void NodeMapView::draw(Rectangle bounds, const std::vector<Entry>& entries, const Font* font)
{
    if (entries.empty()) return;
    const float y = bounds.y + bounds.height / 2.0F;
    const float step = entries.size() > 1 ? bounds.width / static_cast<float>(entries.size() - 1) : 0.0F;

    std::unordered_map<std::string, float> xById;
    for (std::size_t i = 0; i < entries.size(); ++i)
        xById[entries[i].id] = bounds.x + step * static_cast<float>(i);

    // Connections first so node markers draw on top of the lines.
    for (const auto& entry : entries)
    {
        const float x1 = xById[entry.id];
        for (const auto& targetId : entry.connectsTo)
        {
            const auto found = xById.find(targetId);
            if (found == xById.end()) continue;
            DrawLineEx({x1, y}, {found->second, y}, 2.0F, NlmColors::TEXT_DIM);
        }
    }

    const float radius = 10.0F;
    const int labelSize = 12;
    // Labels alternate above/below the line so two adjacent nodes on a
    // crowded strip don't merge into one unreadable block of text.
    const float labelGap = 6.0F;
    for (std::size_t i = 0; i < entries.size(); ++i)
    {
        const auto& entry = entries[i];
        const float x = xById[entry.id];
        const Color color = stateColor(entry);
        if (entry.optional)
            DrawCircleLines(static_cast<int>(x), static_cast<int>(y), radius + 4.0F, color);
        DrawCircle(static_cast<int>(x), static_cast<int>(y), radius, entry.locked ? NlmColors::BG_CARD : color);
        DrawCircleLines(static_cast<int>(x), static_cast<int>(y), radius, color);
        if (entry.current)
            DrawCircleLines(static_cast<int>(x), static_cast<int>(y), radius + 6.0F, NlmColors::AMBER);

        const bool labelBelow = i % 2 == 0;
        const float labelY = labelBelow ? y + radius + labelGap : y - radius - labelGap - static_cast<float>(labelSize);
        const std::string label = fitToWidth(entry.label, step * 1.6F, labelSize, font);
        const float measured = measureWidth(label, labelSize, font);
        const float labelX = x - measured / 2.0F;
        if (font != nullptr)
        {
            DrawTextEx(*font, label.c_str(), {labelX, labelY}, static_cast<float>(labelSize), 1.0F, color);
        }
        else
        {
            DrawText(label.c_str(), static_cast<int>(labelX), static_cast<int>(labelY), labelSize, color);
        }
    }
}
