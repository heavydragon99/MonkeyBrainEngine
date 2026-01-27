
#include "debugDraw/debugDrawSystem.h"
#include <cmath>

bool DebugDrawSystem::initialize() { return true; }

void DebugDrawSystem::update(Registry &aRegistry, float) {
  mContext.clear();

  aRegistry.forEach<Transform, Collider, DebugDraw>(
      [&](Entity, const Transform &t, const Collider &c, const DebugDraw &d) {
        if (!d.isEnabled)
          return;

        std::visit(
            [&](auto &&shape) {
              using T = std::decay_t<decltype(shape)>;

              if constexpr (std::is_same_v<T, BoxCollider>)
                drawBox(t, shape);

              if constexpr (std::is_same_v<T, CircleCollider>)
                drawCircle(t, shape);
            },
            c.shape);
      });
}

void DebugDrawSystem::drawBox(const Transform &t, const BoxCollider &box) {
  vector2 half = {box.width * 0.5f, box.height * 0.5f};

  vector2 p0 = t.position + vector2{-half.x, -half.y};
  vector2 p1 = t.position + vector2{half.x, -half.y};
  vector2 p2 = t.position + vector2{half.x, half.y};
  vector2 p3 = t.position + vector2{-half.x, half.y};

  mContext.line(p0, p1);
  mContext.line(p1, p2);
  mContext.line(p2, p3);
  mContext.line(p3, p0);
}

void DebugDrawSystem::drawCircle(const Transform &t,
                                 const CircleCollider &circle) {
  constexpr int segments = 16;
  float step = 2.0f * 3.1415926f / segments;

  for (int i = 0; i < segments; ++i) {
    float a0 = i * step;
    float a1 = (i + 1) * step;

    vector2 p0 = t.position + vector2{std::cos(a0) * circle.radius,
                                      std::sin(a0) * circle.radius};

    vector2 p1 = t.position + vector2{std::cos(a1) * circle.radius,
                                      std::sin(a1) * circle.radius};

    mContext.line(p0, p1);
  }
}
