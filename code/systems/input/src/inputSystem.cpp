
#include "input/inputSystem.h"

// ------------------------------------------------------------
// Singleton
// ------------------------------------------------------------
InputSystem &InputSystem::Get() {
  static InputSystem instance;
  return instance;
}

// ------------------------------------------------------------
// Initialization
// ------------------------------------------------------------
bool InputSystem::initialize() {
  for (int i = 0; i < static_cast<int>(Key::Count); ++i) {
    mKeyStates[static_cast<Key>(i)] = ButtonState::Up;
  }

  mMouseStates[MouseButton::Left] = ButtonState::Up;
  mMouseStates[MouseButton::Right] = ButtonState::Up;
  mMouseStates[MouseButton::Middle] = ButtonState::Up;

  std::cout << "InputSystem initialized.\n";

  return true;
}

// ------------------------------------------------------------
// Update
// ------------------------------------------------------------
void InputSystem::update(Registry &, float) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    // Handle quit / window events elsewhere if needed
  }

  // --- Keyboard ---
  const bool *keyboard = SDL_GetKeyboardState(nullptr);

  auto keyDown = [&](Key aKey) {
    switch (aKey) {
    case Key::W:
      return keyboard[SDL_SCANCODE_W];
    case Key::A:
      return keyboard[SDL_SCANCODE_A];
    case Key::S:
      return keyboard[SDL_SCANCODE_S];
    case Key::D:
      return keyboard[SDL_SCANCODE_D];
    case Key::Q:
      return keyboard[SDL_SCANCODE_Q];
    case Key::E:
      return keyboard[SDL_SCANCODE_E];
    case Key::Space:
      return keyboard[SDL_SCANCODE_SPACE];
    case Key::Escape:
      return keyboard[SDL_SCANCODE_ESCAPE];
    case Key::LeftShift:
      return keyboard[SDL_SCANCODE_LSHIFT];
    case Key::LeftCtrl:
      return keyboard[SDL_SCANCODE_LCTRL];
    default:
      return false;
    }
  };

  for (auto &[key, state] : mKeyStates) {
    updateButtonState(state, keyDown(key));
  }

  // --- Mouse ---
  Uint32 mouseMask = SDL_GetMouseState(&mMouseX, &mMouseY);

  updateButtonState(mMouseStates[MouseButton::Left],
                    mouseMask & SDL_BUTTON_LMASK);
  updateButtonState(mMouseStates[MouseButton::Right],
                    mouseMask & SDL_BUTTON_RMASK);
  updateButtonState(mMouseStates[MouseButton::Middle],
                    mouseMask & SDL_BUTTON_MMASK);
}

// ------------------------------------------------------------
// Queries
// ------------------------------------------------------------
bool InputSystem::IsKeyPressed(Key aKey) const {
  return mKeyStates.at(aKey) == ButtonState::Pressed;
}

bool InputSystem::IsKeyHeld(Key aKey) const {
  return mKeyStates.at(aKey) == ButtonState::Held;
}

bool InputSystem::IsKeyReleased(Key aKey) const {
  return mKeyStates.at(aKey) == ButtonState::Released;
}

bool InputSystem::IsMousePressed(MouseButton aButton) const {
  return mMouseStates.at(aButton) == ButtonState::Pressed;
}

bool InputSystem::IsMouseHeld(MouseButton aButton) const {
  return mMouseStates.at(aButton) == ButtonState::Held;
}

bool InputSystem::IsMouseReleased(MouseButton aButton) const {
  return mMouseStates.at(aButton) == ButtonState::Released;
}

// ------------------------------------------------------------
// Helpers
// ------------------------------------------------------------
void InputSystem::updateButtonState(ButtonState &aState, bool aIsDown) {
  if (aIsDown) {
    if (aState == ButtonState::Up || aState == ButtonState::Released)
      aState = ButtonState::Pressed;
    else
      aState = ButtonState::Held;
  } else {
    if (aState == ButtonState::Held || aState == ButtonState::Pressed)
      aState = ButtonState::Released;
    else
      aState = ButtonState::Up;
  }
}
