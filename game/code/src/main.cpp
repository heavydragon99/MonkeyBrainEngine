#include <monkeyBrainEngine.h>

int main() {
  MonkeyBrainEngine engine;
  engine.initialize();
  engine.run();
  engine.shutdown();
  return 0;
}
