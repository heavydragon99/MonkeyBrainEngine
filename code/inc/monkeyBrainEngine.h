#ifndef MONKEYBRAINENGINE_H
#define MONKEYBRAINENGINE_H

#include "systems.h"
#include <memory>
#include <vector>

class MonkeyBrainEngine {
public:
  MonkeyBrainEngine() = default;
  ~MonkeyBrainEngine() = default;

  void initialize();
  void shutdown();
  void run(float dt, int frames);
  Registry &getRegistry() { return mRegistry; }

private:
  void addSystem(std::unique_ptr<System> system) {
    mSystems.push_back(std::move(system));
  }

  Registry mRegistry;
  std::vector<std::unique_ptr<System>> mSystems;
};

#endif // MONKEYBRAINENGINE_H
