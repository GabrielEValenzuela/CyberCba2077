#ifndef NLM_COLORS_HPP
#define NLM_COLORS_HPP

/**
 * @file NlmColors.hpp
 * @brief Paleta visual centralizada de Cyber CBA.
 * @details Evita colores hardcodeados en vistas y widgets de raylib.
 */
#include "raylib.h"

// raylib exports these legacy color macros globally. The UI keeps palette names in its namespace.
#ifdef RED
#undef RED
#endif
#ifdef GREEN
#undef GREEN
#endif
#ifdef VIOLET
#undef VIOLET
#endif

namespace NlmColors
{
    constexpr Color BG {13, 15, 26, 255};
    constexpr Color BG_CARD {20, 24, 41, 255};
    constexpr Color CYAN {0, 212, 255, 255};
    constexpr Color VIOLET {124, 58, 237, 255};
    constexpr Color GREEN {16, 185, 129, 255};
    constexpr Color RED {239, 68, 68, 255};
    constexpr Color AMBER {245, 158, 11, 255};
    constexpr Color TEXT {240, 244, 255, 255};
    constexpr Color TEXT_DIM {136, 146, 164, 255};
    constexpr Color B1 = CYAN;
    constexpr Color B2 = VIOLET;
    constexpr Color B3 = GREEN;
    constexpr Color B4 = AMBER;
    constexpr Color B5 = RED;
    constexpr Color B6 = TEXT_DIM;
    constexpr Color B7 = TEXT;
} // namespace NlmColors

#endif // NLM_COLORS_HPP
