#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "system/system.h"
namespace System {
class PhysicsSystem : public System {
public:
  void update(Registry &aRegistry, float aDt) override;
};
} // namespace System
#endif // PHYSICSSYSTEM_H
