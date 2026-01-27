#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <string>
#include <variant>

#include "ecsRegistry.h"

template <typename T> struct ComponentTypeID {
  static size_t id() {
    static size_t mNextID = 0;
    static size_t mID = mNextID++;
    return mID;
  }
};

struct vector2 {
  float x = 0.0f;
  float y = 0.0f;

  constexpr vector2 operator+(const vector2 &aOther) const {
    return {x + aOther.x, y + aOther.y};
  }
  constexpr vector2 operator-(const vector2 &aOther) const {
    return {x - aOther.x, y - aOther.y};
  }
  constexpr vector2 operator*(float aScale) const {
    return {x * aScale, y * aScale};
  }
  constexpr vector2 operator/(float aScale) const {
    return {x / aScale, y / aScale};
  }

  constexpr vector2 &operator+=(const vector2 &aOther) {
    x += aOther.x;
    y += aOther.y;
    return *this;
  }
  constexpr vector2 &operator-=(const vector2 &aOther) {
    x -= aOther.x;
    y -= aOther.y;
    return *this;
  }
  constexpr vector2 &operator*=(float aScale) {
    x *= aScale;
    y *= aScale;
    return *this;
  }
  constexpr vector2 &operator/=(float aScale) {
    x /= aScale;
    y /= aScale;
    return *this;
  }

  constexpr bool operator==(const vector2 &aOther) const {
    return x == aOther.x && y == aOther.y;
  }
  constexpr bool operator!=(const vector2 &aOther) const {
    return !(*this == aOther);
  }

  constexpr float length_squared() const { return x * x + y * y; }
  float length() const { return std::sqrt(length_squared()); }
  vector2 normalized() const {
    float mLength = length();
    return mLength != 0 ? (*this / mLength) : vector2{};
  }
};

struct Transform {
  vector2 position{0.0f, 0.0f};
  float rotation = 0.0f;
  vector2 scale{1.0f, 1.0f};
};

struct Velocity {
  vector2 speed{};
};

struct MovementIntent {
  vector2 speed{0.0f, 0.0f};
};

struct Time {
  float fixedDeltaTime = 0.016f; // 60 FPS
  uint64_t tickCount = 0;
};

struct Active {
  bool enabled = true;
};

struct Lifetime {
  float remainingTime = 0.0f;
};

struct GridPosition {
  int tileX = 0;
  int tileY = 0;
};

struct BoxCollider {
  float width = 1.0f;
  float height = 1.0f;
};

struct CircleCollider {
  float radius = 1.0f;
};

struct Collider {
  int layer = 0;
  int mask = 0;
  std::variant<BoxCollider, CircleCollider> shape;
};

struct Solid {
  bool isSolid = true;
};

struct Trigger {
  bool isTrigger = true;
};

struct Renderable {
  int spriteID = -1;
  int layer = 0;
  bool isVisible = true;
};

struct Animator {
  int currentAnimationID = -1;
  float frameTime = 0.0f;
  bool isPlaying = false;
  bool isLooping = false;
};

struct Name {
  std::string name{};
};

struct DebugDraw {
  bool isEnabled = true;
};

struct Script {
  virtual ~Script() = default;

  virtual void onCreate([[maybe_unused]] Entity aEntity,
                        [[maybe_unused]] Registry &aRegistry) {}

  virtual void onUpdate([[maybe_unused]] Entity aEntity,
                        [[maybe_unused]] Registry &aRegistry,
                        [[maybe_unused]] float aDt) {}

  virtual void onFixedUpdate([[maybe_unused]] Entity aEntity,
                             [[maybe_unused]] Registry &aRegistry,
                             [[maybe_unused]] float aFixedDt) {}

  virtual void onDestroy([[maybe_unused]] Entity aEntity,
                         [[maybe_unused]] Registry &aRegistry) {}
};

struct ScriptComponent {
  std::unique_ptr<Script> instance;
  bool enabled = true;
  bool initialized = false;

  ScriptComponent(std::unique_ptr<Script> script)
      : instance(std::move(script)) {}
};

#endif // COMPONENTS_H
