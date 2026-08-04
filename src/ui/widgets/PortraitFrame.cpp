#include "ui/widgets/PortraitFrame.hpp"

#include "ui/NlmColors.hpp"

void PortraitFrame::draw(Rectangle bounds, const Texture2D* portrait, Color accent)
{
    DrawRectangleRec(bounds, NlmColors::BG_CARD);
    DrawRectangleLinesEx(bounds, 2.0F, accent);
    if (portrait == nullptr || portrait->id == 0) return;

    // The prologue runtime portraits are 4:5 canvases with transparent space
    // above the shoulders; crop their authored lower bust region. Other
    // callers retain the legacy full-body top-third fallback.
    const bool isDedicatedPortrait = portrait->height == portrait->width * 5 / 4;
    const float cropHeight = static_cast<float>(portrait->height) * (isDedicatedPortrait ? 0.56F : 0.34F);
    const float cropY = isDedicatedPortrait ? static_cast<float>(portrait->height) - cropHeight : 0.0F;
    const Rectangle src {0, cropY, static_cast<float>(portrait->width), cropHeight};
    const float srcAspect = src.width / src.height;
    const float dstAspect = bounds.width / bounds.height;
    Rectangle dest = bounds;
    if (srcAspect > dstAspect)
    {
        dest.width = bounds.height * srcAspect;
        dest.x = bounds.x - (dest.width - bounds.width) / 2.0F;
    }
    else
    {
        dest.height = bounds.width / srcAspect;
        dest.y = bounds.y - (dest.height - bounds.height) / 2.0F;
    }
    BeginScissorMode(static_cast<int>(bounds.x), static_cast<int>(bounds.y), static_cast<int>(bounds.width), static_cast<int>(bounds.height));
    DrawTexturePro(*portrait, src, dest, {0, 0}, 0.0F, WHITE);
    EndScissorMode();
    DrawRectangleLinesEx(bounds, 2.0F, accent);
}
