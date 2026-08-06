#ifndef PORTRAIT_FRAME_HPP
#define PORTRAIT_FRAME_HPP

/**
 * @file PortraitFrame.hpp
 * @brief Bordered frame for a head-and-shoulders dialogue/interaction
 * portrait. Presentation only.
 * @details Dedicated 4:5 portrait assets use their lower alpha-safe bust
 * region. Legacy full-body sprites retain the top-third fallback crop.
 */
#include "raylib.h"

class PortraitFrame
{
public:
    static void draw(Rectangle bounds, const Texture2D* portrait, Color accent);
};

#endif // PORTRAIT_FRAME_HPP
