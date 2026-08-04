#ifndef UI_METRICS_HPP
#define UI_METRICS_HPP

/**
 * @file UiMetrics.hpp
 * @brief Centralized layout constants for the cinematic UI pivot (ADR 0017).
 * Screens previously used small, conservative panel bounds (e.g. 970x525
 * inside a 1280x720 canvas) leaving large unused black margins — the exact
 * complaint the PoC screenshots called out. These constants make every
 * screen fill the canvas edge-to-edge (minus a small safe margin) instead
 * of each draw* function picking its own ad hoc box.
 */
#include "raylib.h"

namespace UiMetrics
{
constexpr float margin = 20.0F;
constexpr float canvasWidth = 1280.0F;
constexpr float canvasHeight = 720.0F;

// Shared header row: mission progress bar (left/center) + current objective (right).
constexpr float headerY = margin;
constexpr float headerHeight = 110.0F;

// Main content area, below the header, filling the rest of the canvas.
constexpr float contentY = headerY + headerHeight + 10.0F;
constexpr float contentHeight = canvasHeight - contentY - margin;
constexpr float contentX = margin;
constexpr float contentWidth = canvasWidth - margin * 2.0F;

// Bottom panel (dialogue/interaction) height within the content area; the
// cinematic scene above it takes the remaining space.
constexpr float bottomPanelHeight = 230.0F;
constexpr float sceneHeight = contentHeight - bottomPanelHeight - 10.0F;

constexpr Rectangle contentBounds() { return {contentX, contentY, contentWidth, contentHeight}; }
constexpr Rectangle sceneBounds() { return {contentX, contentY, contentWidth, sceneHeight}; }
constexpr Rectangle bottomPanelBounds() { return {contentX, contentY + sceneHeight + 10.0F, contentWidth, bottomPanelHeight}; }
} // namespace UiMetrics

#endif // UI_METRICS_HPP
