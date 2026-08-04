#ifndef TEXT_WRAP_HPP
#define TEXT_WRAP_HPP

/**
 * @file TextWrap.hpp
 * @brief Greedy word-wrap shared by GameApp and the reusable story/dialogue
 * widgets, so wrapped line breaks always match what DrawTextEx actually
 * renders (measures against the real UI font when available).
 */
#include <string>
#include <vector>

#include "raylib.h"

std::vector<std::string> wrapText(const std::string& text, int maxWidth, int fontSize, const Font* font);

#endif // TEXT_WRAP_HPP
