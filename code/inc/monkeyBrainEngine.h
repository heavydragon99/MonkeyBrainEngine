#ifndef MONKEYBRAINENGINE_H
#define MONKEYBRAINENGINE_H

#include "components.h"
#include "system/system.h"
#include <memory>
#include <vector>

class MonkeyBrainEngine {
public:
  MonkeyBrainEngine() = default;
  ~MonkeyBrainEngine() = default;

  void initialize();
  void shutdown();
  void run(float aDt, int aFrames);
  Registry &getRegistry() { return mRegistry; }

private:
  void addSystem(std::unique_ptr<System::System> aSystem);

  Registry mRegistry;
  std::vector<std::unique_ptr<System::System>> mSystems;
};

#endif // MONKEYBRAINENGINE_H
