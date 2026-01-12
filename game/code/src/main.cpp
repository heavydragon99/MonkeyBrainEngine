
#include "monkeyBrainEngine.h"

int main() {
  MonkeyBrainEngine engine;
  Registry &registry = engine.getRegistry();

  Entity player = registry.createEntity();
  Entity enemy = registry.createEntity();
  Entity npc = registry.createEntity();

  registry.add(player, Transform{{0, 0}, 0, {1, 1}});
  registry.add(player, Health{50, 100});
  registry.add(player, Velocity{{1, 2}});

  registry.add(enemy, Transform{{10, 10}, 0, {1, 1}});
  registry.add(enemy, Health{100, 100});

  registry.add(npc, Transform{{5, 5}, 0, {1, 1}});

  engine.initialize();
  engine.run(1.0f, 3);
  engine.shutdown();
}
