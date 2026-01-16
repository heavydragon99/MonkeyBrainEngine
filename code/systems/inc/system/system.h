#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "ecsRegistry.h"
namespace System {
class System {
public:
  virtual ~System() = default;
  virtual void update(Registry &registry, float dt) = 0;
};
} // namespace System
#endif // SYSTEMS_H
