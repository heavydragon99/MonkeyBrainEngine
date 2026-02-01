#ifndef MONKEYBRAINENGINE_H
#define MONKEYBRAINENGINE_H

#include <memory>
#include <vector>

#include "components.h"
#include "ecsRegistry.h"
#include "graphics/graphicsSystem.h"
#include "input/inputSystem.h"
#include "physics/physicsSystem.h"
#include "scriptHelper.h"
#include "scripts/scriptsSystem.h"
#include "system/system.h"

class MonkeyBrainEngine {
public:
  MonkeyBrainEngine() = default;
  ~MonkeyBrainEngine() = default;

  void initialize();
  void shutdown();
  void run();
  Registry &getRegistry() { return mRegistry; }

private:
  void addSystem(std::unique_ptr<System> aSystem);

  // TODO: make unique ptrs from registry and systems
  // todo: make this class a singleton
  Registry mRegistry;
  std::vector<std::unique_ptr<System>> mSystems;
  GraphicsSystem *mGraphicsSystem = nullptr;
  PhysicsSystem *mPhysicsSystem = nullptr;
  ScriptsSystem *mScriptsSystem = nullptr;
};

#endif // MONKEYBRAINENGINE_H
