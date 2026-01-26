#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include <SDL3/SDL.h>
#include <unordered_map>

#include "input/inputKeys.h"
#include "system/system.h"

class InputSystem final : public System {
public:
  static InputSystem &Get();

  bool initialize() override;
  void update(Registry &aRegistry, float aDt) override;

  // Keyboard
  bool IsKeyPressed(Key aKey) const;
  bool IsKeyHeld(Key aKey) const;
  bool IsKeyReleased(Key aKey) const;

  // Mouse
  bool IsMousePressed(MouseButton aButton) const;
  bool IsMouseHeld(MouseButton aButton) const;
  bool IsMouseReleased(MouseButton aButton) const;

  float GetMouseX() const { return mMouseX; }
  float GetMouseY() const { return mMouseY; }

private:
  InputSystem() = default;
  ~InputSystem() override = default;

  InputSystem(const InputSystem &) = delete;
  InputSystem &operator=(const InputSystem &) = delete;

  void updateButtonState(ButtonState &aState, bool aIsDown);

  std::unordered_map<Key, ButtonState> mKeyStates;
  std::unordered_map<MouseButton, ButtonState> mMouseStates;

  float mMouseX = 0;
  float mMouseY = 0;
};
#endif // INPUTSYSTEM_H
