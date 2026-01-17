#include "monkeyBrainEngine.h"

#include <SDL3/SDL_video.h>
#include <iostream>

int main() {
  MonkeyBrainEngine engine;
  Registry &registry = engine.getRegistry();

  Entity player = registry.createEntity();
  Entity enemy = registry.createEntity();
  Entity npc = registry.createEntity();

  registry.add(player, Transform{{0, 0}, 0, {1, 1}});
  registry.add(player, Velocity{{1, 2}});

  registry.add(enemy, Transform{{10, 10}, 0, {1, 1}});

  registry.add(npc, Transform{{5, 5}, 0, {1, 1}});
  try {
    engine.initialize();
  } catch (const std::exception &e) {
    std::cerr << "Failed to initialize engine: " << e.what() << "\n";
    return -1;
  }
  try {
    engine.run();
  } catch (const std::exception &e) {
    std::cerr << "Error during engine run: " << e.what() << "\n";
  }
  engine.shutdown();
}
