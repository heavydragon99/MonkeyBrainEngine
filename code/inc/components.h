#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <cmath>
#include <cstddef>

template <typename T> struct ComponentTypeID {
  static size_t id() {
    static size_t id = nextID++;
    return id;
  }

private:
  static inline size_t nextID = 0;
};

struct vector2 {
  float x = 0.0f;
  float y = 0.0f;

  constexpr vector2 operator+(const vector2 &o) const {
    return {x + o.x, y + o.y};
  }
  constexpr vector2 operator-(const vector2 &o) const {
    return {x - o.x, y - o.y};
  }
  constexpr vector2 operator*(float s) const { return {x * s, y * s}; }
  constexpr vector2 operator/(float s) const { return {x / s, y / s}; }

  constexpr vector2 &operator+=(const vector2 &o) {
    x += o.x;
    y += o.y;
    return *this;
  }
  constexpr vector2 &operator-=(const vector2 &o) {
    x -= o.x;
    y -= o.y;
    return *this;
  }
  constexpr vector2 &operator*=(float s) {
    x *= s;
    y *= s;
    return *this;
  }
  constexpr vector2 &operator/=(float s) {
    x /= s;
    y /= s;
    return *this;
  }

  constexpr bool operator==(const vector2 &o) const {
    return x == o.x && y == o.y;
  }
  constexpr bool operator!=(const vector2 &o) const { return !(*this == o); }

  constexpr float length_squared() const { return x * x + y * y; }
  float length() const { return std::sqrt(length_squared()); }
  vector2 normalized() const {
    float l = length();
    return l != 0 ? (*this / l) : vector2{};
  }
};

struct Transform {
  vector2 position{};
  float rotation = 0.0f;
  vector2 scale{1.0f, 1.0f};
};

struct Velocity {
  vector2 speed{};
};

struct Health {
  int current = 0;
  int max = 0;
};

#endif // COMPONENTS_H
