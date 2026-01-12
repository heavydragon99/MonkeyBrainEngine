#include "monkeyBrainEngine.h"
#include <iostream>

void MonkeyBrainEngine::initialize() {
  addSystem(std::make_unique<PhysicsSystem>());
  addSystem(std::make_unique<HealthSystem>());
}

void MonkeyBrainEngine::shutdown() { std::cout << "Engine shutdown.\n"; }

void MonkeyBrainEngine::run(float aDt, int aFrames) {
  std::cout << "Engine running...\n";
  for (int i = 0; i < aFrames; ++i) {
    for (auto &sys : mSystems) {
      sys->update(mRegistry, aDt);
    }
  }
}
