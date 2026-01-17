#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "system/system.h"
class PhysicsSystem : public System {
public:
  bool initialize() override;
  void update(Registry &aRegistry, float aDt) override;
};
#endif // PHYSICSSYSTEM_H
