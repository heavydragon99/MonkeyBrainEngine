#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <string>
#include <variant>

template <typename T> struct ComponentTypeID {
  static size_t id() {
    static size_t mNextID = 0;
    static size_t mID = mNextID++;
    return mID;
  }
};

struct vector2 {
  float mX = 0.0f;
  float mY = 0.0f;

  constexpr vector2 operator+(const vector2 &aOther) const {
    return {mX + aOther.mX, mY + aOther.mY};
  }
  constexpr vector2 operator-(const vector2 &aOther) const {
    return {mX - aOther.mX, mY - aOther.mY};
  }
  constexpr vector2 operator*(float aScale) const {
    return {mX * aScale, mY * aScale};
  }
  constexpr vector2 operator/(float aScale) const {
    return {mX / aScale, mY / aScale};
  }

  constexpr vector2 &operator+=(const vector2 &aOther) {
    mX += aOther.mX;
    mY += aOther.mY;
    return *this;
  }
  constexpr vector2 &operator-=(const vector2 &aOther) {
    mX -= aOther.mX;
    mY -= aOther.mY;
    return *this;
  }
  constexpr vector2 &operator*=(float aScale) {
    mX *= aScale;
    mY *= aScale;
    return *this;
  }
  constexpr vector2 &operator/=(float aScale) {
    mX /= aScale;
    mY /= aScale;
    return *this;
  }

  constexpr bool operator==(const vector2 &aOther) const {
    return mX == aOther.mX && mY == aOther.mY;
  }
  constexpr bool operator!=(const vector2 &aOther) const {
    return !(*this == aOther);
  }

  constexpr float length_squared() const { return mX * mX + mY * mY; }
  float length() const { return std::sqrt(length_squared()); }
  vector2 normalized() const {
    float mLength = length();
    return mLength != 0 ? (*this / mLength) : vector2{};
  }
};

struct Transform {
  vector2 mPosition{0.0f, 0.0f};
  float mRotation = 0.0f;
  vector2 mScale{1.0f, 1.0f};
};

struct Velocity {
  vector2 mSpeed{};
};

struct MovementIntent {
  vector2 mSpeed{0.0f, 0.0f};
};

struct Time {
  float mFixedDeltaTime = 0.016f; // 60 FPS
  uint64_t mTickCount = 0;
};

struct Active {
  bool mEnabled = true;
};

struct Lifetime {
  float mRemainingTime = 0.0f;
};

struct GridPosition {
  int mTileX = 0;
  int mTileY = 0;
};

struct BoxCollider {
  float mWidth = 1.0f;
  float mHeight = 1.0f;
};

struct CircleCollider {
  float mRadius = 1.0f;
};

struct Collider {
  int mLayer = 0;
  int mMask = 0;
  std::variant<BoxCollider, CircleCollider> mShape;
};

struct Solid {
  bool mIsSolid = true;
};

struct Trigger {
  bool mIsTrigger = true;
};

struct Renderable {
  int mSpriteID = -1;
  int mLayer = 0;
  bool mIsVisible = true;
};

struct Animator {
  int mCurrentAnimationID = -1;
  float mFrameTime = 0.0f;
  bool mIsPlaying = false;
  bool mIsLooping = false;
};

struct Name {
  std::string mName{};
};

struct DebugDraw {
  bool IsEnabled = true;
};

#endif // COMPONENTS_H
