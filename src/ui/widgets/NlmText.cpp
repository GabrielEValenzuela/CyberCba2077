#include "ui/widgets/NlmText.hpp"

void NlmText::draw(const char* text, int x, int y, int size, Color color)
{
    DrawText(text, x, y, size, color);
}

void NlmText::drawCentered(const char* text, int y, int size, Color color)
{
    const int x = (GetScreenWidth() - MeasureText(text, size)) / 2;
    DrawText(text, x, y, size, color);
}
