#ifndef MONKEYBRAINENGINE_H
#define MONKEYBRAINENGINE_H

#include <memory>
#include <vector>

#include "components.h"
#include "graphics/graphicsSystem.h"
#include "physics/physicsSystem.h"
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

  Registry mRegistry;
  std::vector<std::unique_ptr<System>> mSystems;
  GraphicsSystem *mGraphicsSystem = nullptr;
  PhysicsSystem *mPhysicsSystem = nullptr;
};

#endif // MONKEYBRAINENGINE_H
