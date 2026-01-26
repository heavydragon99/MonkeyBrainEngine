#ifndef INPUTKEYS_H
#define INPUTKEYS_H

enum class Key {
  Unknown = 0,

  W,
  A,
  S,
  D,
  Q,
  E,
  Space,
  Escape,
  LeftShift,
  LeftCtrl,

  Count
};

enum class MouseButton { Left, Right, Middle };

enum class ButtonState { Up, Pressed, Held, Released };

#endif // INPUTKEYS_H
