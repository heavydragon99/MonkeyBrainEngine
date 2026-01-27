#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

#include "components.h"
#include <vector>

struct DebugLine {
  vector2 a;
  vector2 b;
  vector3 color;
};

struct DebugDrawContext {
  std::vector<DebugLine> lines;

  void clear() { lines.clear(); }

  void line(vector2 a, vector2 b, vector3 color = {1, 1, 1}) {
    lines.push_back({a, b, color});
  }
};

struct DebugVertex {
  float x, y;
  float r, g, b;
};

#endif // DEBUGDRAW_H
