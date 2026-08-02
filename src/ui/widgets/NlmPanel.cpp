#include "ui/widgets/NlmPanel.hpp"

#include "ui/NlmColors.hpp"

void NlmPanel::draw(Rectangle bounds, Color border)
{
    DrawRectangleRec(bounds, NlmColors::BG_CARD);
    DrawRectangleLinesEx(bounds, 2.0F, border);
}
