#pragma once
#include "raylib.h"
#include <array>
enum class InputAction
{
    NavigateUp,
    NavigateDown,
    NavigateLeft,
    NavigateRight,
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    Interact,
    PrimaryAttack,
    SecondaryAttack,
    Dodge,
    Confirm,
    Cancel,
    Pause,
    OpenObjectives,
    ToggleDevOverlay,
    Count
};
enum class InputDevice
{
    Keyboard,
    Mouse,
    Gamepad
};
class InputRouter
{
public:
    void update();
    bool wasPressed(InputAction action) const;
    bool wasDown(InputAction action) const;
    InputDevice lastDevice() const;
    Vector2 virtualMouse(Vector2 offset, float scale) const;

private:
    std::array<bool, static_cast<int>(InputAction::Count)> m_pressed {};
    std::array<bool, static_cast<int>(InputAction::Count)> m_down {};
    InputDevice m_lastDevice {InputDevice::Keyboard};
};
