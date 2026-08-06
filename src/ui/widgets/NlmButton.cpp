#include "ui/widgets/NlmButton.hpp"

#include "ui/NlmColors.hpp"
#include "ui/widgets/NlmPanel.hpp"

NlmButton::NlmButton(Rectangle bounds, const char* label)
    : m_bounds(bounds)
    , m_label(label)
{
}

void NlmButton::draw(bool focused) const
{
    const Color border = focused ? NlmColors::CYAN : NlmColors::TEXT_DIM;
    NlmPanel::draw(m_bounds, border);
    const int size = 24;
    const int textX = static_cast<int>(m_bounds.x + (m_bounds.width - MeasureText(m_label, size)) / 2.0F);
    const int textY = static_cast<int>(m_bounds.y + (m_bounds.height - size) / 2.0F);
    DrawText(m_label, textX, textY, size, focused ? NlmColors::TEXT : NlmColors::TEXT_DIM);
}

bool NlmButton::isHovered() const
{
    return CheckCollisionPointRec(GetMousePosition(), m_bounds);
}
