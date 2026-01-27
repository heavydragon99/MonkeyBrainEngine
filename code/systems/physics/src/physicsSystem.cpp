#include "physics/physicsSystem.h"

#include <iostream>

#include "components.h"

bool PhysicsSystem::initialize() {
  std::cout << "PhysicsSystem initialized.\n";
  return true;
}

void PhysicsSystem::update(Registry &aRegistry, float aDt) {
  aRegistry.forEach<Transform, Velocity>([aDt]([[maybe_unused]] Entity aEntity,
                                               Transform &aTransform,
                                               Velocity &aVelocity) {
    aTransform.position += aVelocity.speed * aDt;
  });
}
