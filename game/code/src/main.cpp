#include "components.h"
#include "monkeyBrainEngine.h"

#include <iostream>

class PlayerScript : public Script {
  void onCreate(Entity aEntity, Registry &aRegistry) override {
    std::cout << "PlayerScript created for entity " << aEntity << "\n";
  }

  void onUpdate(Entity aEntity, Registry &aRegistry, float aDt) override {
    InputSystem &input = InputSystem::Get();

    if (input.IsKeyHeld(Key::W)) {
      std::cout << "Entity " << aEntity << " moved up\n";
    }

    if (input.IsKeyHeld(Key::S)) {
      std::cout << "Entity " << aEntity << " moved down\n";
    }

    if (input.IsKeyHeld(Key::A)) {
      std::cout << "Entity " << aEntity << " moved left\n";
    }

    if (input.IsKeyHeld(Key::D)) {
      std::cout << "Entity " << aEntity << " moved right\n";
    }
  }
};

int main() {
  MonkeyBrainEngine engine;
  Registry &registry = engine.getRegistry();

  Entity player = registry.createEntity();
  Entity enemy = registry.createEntity();
  Entity npc = registry.createEntity();

  registry.add(player, Transform{{0, 0}, 0, {1, 1}});
  registry.add(player, Velocity{{1, 2}});
  addScript<PlayerScript>(registry, player);

  Transform enemyTransform{{10, 10}, 0, {1, 1}};
  registry.add(enemy, enemyTransform);

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
