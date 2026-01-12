#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "ecsRegistry.h"
#include <iostream>

#include "components.h"

class System {
public:
  virtual ~System() = default;
  virtual void update(Registry &registry, float dt) = 0;
};

class PhysicsSystem : public System {
public:
  void update(Registry &registry, float dt) override {
    registry.forEach<Transform, Velocity>(
        [dt](Transform &t, Velocity &v) { t.position += v.speed * dt; });
  }
};

class HealthSystem : public System {
public:
  void update(Registry &registry, float /*dt*/) override {
    registry.forEach<Health>([](Health &h) {
      if (h.current == h.max)
        std::cout << "Entity is at full health!\n";
      else
        std::cout << "Entity health: " << h.current << "/" << h.max << "\n";
    });
  }
};

#endif // SYSTEMS_H
