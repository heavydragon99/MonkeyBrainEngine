#include "monkeyBrainEngine.h"

#include <iostream>

#include "physics/physicsSystem.h"

void MonkeyBrainEngine::initialize() {
  addSystem(std::make_unique<System::PhysicsSystem>());
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

void MonkeyBrainEngine::addSystem(std::unique_ptr<System::System> aSystem) {
  mSystems.push_back(std::move(aSystem));
}
