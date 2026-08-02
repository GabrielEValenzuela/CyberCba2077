#include "ui/InputRouter.hpp"
namespace
{
    constexpr int index(InputAction action)
    {
        return static_cast<int>(action);
    }
} // namespace
void InputRouter::update()
{
    m_pressed.fill(false);
    m_down.fill(false);
    auto key = [&](InputAction action, int code)
    {
        if (IsKeyPressed(code))
        {
            m_pressed[index(action)] = true;
            m_lastDevice = InputDevice::Keyboard;
        }
        if (IsKeyDown(code))
            m_down[index(action)] = true;
    };
    key(InputAction::NavigateUp, KEY_UP);
    key(InputAction::NavigateDown, KEY_DOWN);
    key(InputAction::NavigateLeft, KEY_LEFT);
    key(InputAction::NavigateRight, KEY_RIGHT);
    key(InputAction::MoveUp, KEY_W);
    key(InputAction::MoveDown, KEY_S);
    key(InputAction::MoveLeft, KEY_A);
    key(InputAction::MoveRight, KEY_D);
    key(InputAction::Interact, KEY_E);
    key(InputAction::PrimaryAttack, KEY_X);
    key(InputAction::SecondaryAttack, KEY_C);
    key(InputAction::Dodge, KEY_LEFT_SHIFT);
    key(InputAction::Confirm, KEY_ENTER);
    key(InputAction::Confirm, KEY_SPACE);
    key(InputAction::Cancel, KEY_ESCAPE);
    key(InputAction::Pause, KEY_P);
    key(InputAction::OpenObjectives, KEY_O);
    key(InputAction::ToggleDevOverlay, KEY_F3);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        m_pressed[index(InputAction::Confirm)] = true;
        m_lastDevice = InputDevice::Mouse;
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        m_down[index(InputAction::Confirm)] = true;
    if (IsGamepadAvailable(0))
    {
        auto pad = [&](InputAction action, int button)
        {
            if (IsGamepadButtonPressed(0, button))
            {
                m_pressed[index(action)] = true;
                m_lastDevice = InputDevice::Gamepad;
            }
        };
        pad(InputAction::Confirm, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
        pad(InputAction::Cancel, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);
        pad(InputAction::Interact, GAMEPAD_BUTTON_RIGHT_FACE_LEFT);
        pad(InputAction::PrimaryAttack, GAMEPAD_BUTTON_RIGHT_FACE_UP);
        pad(InputAction::Pause, GAMEPAD_BUTTON_MIDDLE_RIGHT);
        pad(InputAction::NavigateUp, GAMEPAD_BUTTON_LEFT_FACE_UP);
        pad(InputAction::NavigateDown, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
        pad(InputAction::NavigateLeft, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
        pad(InputAction::NavigateRight, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
        if (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) < -0.5F) m_down[index(InputAction::MoveLeft)] = true;
        if (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) > 0.5F) m_down[index(InputAction::MoveRight)] = true;
        if (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) < -0.5F) m_down[index(InputAction::MoveUp)] = true;
        if (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) > 0.5F) m_down[index(InputAction::MoveDown)] = true;
    }
}
bool InputRouter::wasPressed(InputAction action) const
{
    return m_pressed[index(action)];
}
bool InputRouter::wasDown(InputAction action) const
{
    return m_down[index(action)];
}
InputDevice InputRouter::lastDevice() const
{
    return m_lastDevice;
}
Vector2 InputRouter::virtualMouse(Vector2 offset, float scale) const
{
    Vector2 p = GetMousePosition();
    return {(p.x - offset.x) / scale, (p.y - offset.y) / scale};
}
