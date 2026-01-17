#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "ecsRegistry.h"
class System {
public:
  virtual ~System() = default;
  virtual bool initialize() = 0;
  virtual void update(Registry &aRegistry, float aDt) = 0;
};
#endif // SYSTEMS_H
