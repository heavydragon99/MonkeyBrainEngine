#ifndef GRAPHICSYSTEM_H
#define GRAPHICSYSTEM_H

#include "system/system.h"

class GraphicsSystem : public System {
public:
  virtual ~GraphicsSystem() = default;

  virtual bool initialize() = 0;
  virtual bool initialize(int aWidth, int aHeight) = 0;
  virtual void shutdown() = 0;

  virtual bool shouldQuit() const = 0;
};
#endif // GRAPHICSYSTEM_H
