#include "ui/widgets/ParticipantPanel.hpp"

#include <algorithm>
#include <string>

#include "ui/NlmColors.hpp"
#include "ui/widgets/NlmPanel.hpp"
#include "ui/widgets/StatusPanel.hpp"

namespace
{
void drawBar(Rectangle bounds, float ratio, Color fill)
{
    DrawRectangleRec(bounds, NlmColors::BG_CARD);
    const Rectangle filled {bounds.x, bounds.y, bounds.width * std::clamp(ratio, 0.0F, 1.0F), bounds.height};
    DrawRectangleRec(filled, fill);
    DrawRectangleLinesEx(bounds, 1.0F, NlmColors::TEXT_DIM);
}
} // namespace

void ParticipantPanel::draw(Rectangle bounds, const cybercba::EncounterParticipant& participant, bool active, Color accent, const Font* font)
{
    NlmPanel::draw(bounds, active ? NlmColors::CYAN : accent);
    const float pad = 10.0F;
    const int nameSize = 18;
    const std::string name = participant.displayName + (active ? "  *" : "");
    if (font != nullptr)
    {
        DrawTextEx(*font, name.c_str(), {bounds.x + pad, bounds.y + pad}, static_cast<float>(nameSize), 1.0F, accent);
    }
    else
    {
        DrawText(name.c_str(), static_cast<int>(bounds.x + pad), static_cast<int>(bounds.y + pad), nameSize, accent);
    }

    const float barY = bounds.y + pad + static_cast<float>(nameSize) + 6.0F;
    const float barWidth = bounds.width - pad * 2.0F;
    const float integrityRatio = participant.maxIntegrity > 0 ? static_cast<float>(participant.integrity) / static_cast<float>(participant.maxIntegrity) : 0.0F;
    drawBar({bounds.x + pad, barY, barWidth, 14.0F}, integrityRatio, NlmColors::GREEN);
    const std::string integrityLabel = std::to_string(participant.integrity) + "/" + std::to_string(participant.maxIntegrity);
    const int smallSize = 13;
    if (font != nullptr)
    {
        DrawTextEx(*font, integrityLabel.c_str(), {bounds.x + pad + 4.0F, barY + 1.0F}, static_cast<float>(smallSize), 1.0F, NlmColors::TEXT);
    }
    else
    {
        DrawText(integrityLabel.c_str(), static_cast<int>(bounds.x + pad + 4.0F), static_cast<int>(barY + 1.0F), smallSize, NlmColors::TEXT);
    }

    float nextY = barY + 14.0F + 6.0F;
    if (participant.resourcePool > 0 || !participant.resourceName.empty())
    {
        const std::string resourceLabel = participant.resourceName + ": " + std::to_string(participant.resourcePool);
        if (font != nullptr)
        {
            DrawTextEx(*font, resourceLabel.c_str(), {bounds.x + pad, nextY}, static_cast<float>(smallSize), 1.0F, NlmColors::TEXT_DIM);
        }
        else
        {
            DrawText(resourceLabel.c_str(), static_cast<int>(bounds.x + pad), static_cast<int>(nextY), smallSize, NlmColors::TEXT_DIM);
        }
        nextY += static_cast<float>(smallSize) + 6.0F;
    }

    if (!participant.statuses.empty())
        StatusPanel::draw({bounds.x + pad, nextY, barWidth, 18.0F}, participant.statuses, font);
}
