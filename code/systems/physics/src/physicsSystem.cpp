#include "physics/physicsSystem.h"

#include <iostream>

#include "components.h"

bool PhysicsSystem::initialize() {
  std::cout << "PhysicsSystem initialized.\n";
  return true;
}

void PhysicsSystem::update(Registry &aRegistry, float aDt) {
  aRegistry.forEach<Transform, Velocity>(
      [aDt](Transform &aTransform, Velocity &aVelocity) {
        aTransform.mPosition += aVelocity.mSpeed * aDt;
        std::cout << "Updated entity to position (" << aTransform.mPosition.mX
                  << ", " << aTransform.mPosition.mY << ")\n";
      });
}
