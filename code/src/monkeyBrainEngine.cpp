#include "monkeyBrainEngine.h"

#include <iostream>

#include "graphics/graphicsBackend.h"
#include "input/inputSystem.h"
#include "scripts/scriptsSystem.h"

void MonkeyBrainEngine::initialize() {
  std::cout << "Using graphics backend: " << typeid(ActiveGraphicsSystem).name()
            << "\n";

  auto graphics = std::make_unique<ActiveGraphicsSystem>();
  mGraphicsSystem = graphics.get();
  addSystem(std::move(graphics));

  auto physics = std::make_unique<PhysicsSystem>();
  mPhysicsSystem = physics.get();
  addSystem(std::move(physics));

  auto scripts = std::make_unique<ScriptsSystem>();
  mScriptsSystem = scripts.get();
  addSystem(std::move(scripts));

  InputSystem::Get().initialize();

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

  if (mPhysicsSystem) {
    mPhysicsSystem = nullptr;
  }

  mSystems.clear();
  std::cout << "Engine shutdown.\n";
}

void MonkeyBrainEngine::run() {
  std::cout << "Engine running (max 60 FPS)...\n";

  const double targetFrameTime = 1.0 / 60.0; // 60 FPS
  double accumulator = 0.0;

  Uint64 prevCounter = SDL_GetPerformanceCounter();
  double fpsTimer = 0.0;
  int frames = 0;

  bool running = true;

  while (running && !mGraphicsSystem->shouldQuit()) {
    // --- Compute delta time ---
    Uint64 currentCounter = SDL_GetPerformanceCounter();
    double deltaTime =
        double(currentCounter - prevCounter) / SDL_GetPerformanceFrequency();
    prevCounter = currentCounter;

    accumulator += deltaTime;

    // --- Poll input / events ---
    if (mGraphicsSystem) {
      // Exit if window closed or Esc pressed
      const bool *keyboardState = SDL_GetKeyboardState(nullptr);
      if (mGraphicsSystem->shouldQuit() || keyboardState[SDL_SCANCODE_ESCAPE]) {
        running = false;
        break;
      }
    }

    // --- Physics system ---
    mPhysicsSystem->update(mRegistry, static_cast<float>(deltaTime));

    // --- Graphics / Render system ---
    mGraphicsSystem->update(mRegistry, static_cast<float>(deltaTime));

    // --- Scripts system ---
    mScriptsSystem->update(mRegistry, static_cast<float>(deltaTime));

    // --- Input system update ---
    InputSystem::Get().update(mRegistry, static_cast<float>(deltaTime));

    // --- Limit to 60 FPS ---
    Uint64 frameEndCounter = SDL_GetPerformanceCounter();
    double frameTime = double(frameEndCounter - currentCounter) /
                       SDL_GetPerformanceFrequency();
    if (frameTime < targetFrameTime) {
      SDL_Delay(Uint32((targetFrameTime - frameTime) * 1000.0));
    }
    // TODO: Seperate a fixed update and variable update (physics fixed,
    // animation variable)
  }
}

void MonkeyBrainEngine::addSystem(std::unique_ptr<System> aSystem) {
  mSystems.push_back(std::move(aSystem));
}
