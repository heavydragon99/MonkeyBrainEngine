#include "monkeyBrainEngine.h"

#include <iostream>

#include "graphics/graphicsBackend.h"

void MonkeyBrainEngine::initialize() {
  std::cout << "Using graphics backend: " << typeid(ActiveGraphicsSystem).name()
            << "\n";

  auto graphics = std::make_unique<ActiveGraphicsSystem>();
  mGraphicsSystem = graphics.get();
  addSystem(std::move(graphics));

  auto physics = std::make_unique<PhysicsSystem>();
  mPhysicsSystem = physics.get();
  addSystem(std::move(physics));

  for (auto &sys : mSystems) {
    if (!sys->initialize()) {
      std::cerr << "Failed to initialize a system.\n";
      return;
    }
  }
}

void MonkeyBrainEngine::shutdown() {
  if (mGraphicsSystem) {
    mGraphicsSystem->shutdown();
    mGraphicsSystem = nullptr;
  }

  mSystems.clear();
  std::cout << "Engine shutdown.\n";
}

void MonkeyBrainEngine::run(float aDt, int aFrames) {
  std::cout << "Engine running...\n";

  for (int i = 0; i < aFrames; ++i) {

    if (mGraphicsSystem && mGraphicsSystem->shouldQuit())
      break;

    for (auto &sys : mSystems) {
      sys->update(mRegistry, aDt);
    }
  }
}

void MonkeyBrainEngine::addSystem(std::unique_ptr<System> aSystem) {
  mSystems.push_back(std::move(aSystem));
}
