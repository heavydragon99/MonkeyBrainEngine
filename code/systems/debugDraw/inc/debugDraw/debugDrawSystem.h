#ifndef DEBUGDRAWSYSTEM_H
#define DEBUGDRAWSYSTEM_H

#include "components.h"
#include "debugDraw.h"
#include "system/system.h"

class DebugDrawSystem : public System {
public:
  bool initialize() override;
  void update(Registry &aRegistry, float aDt) override;

  const DebugDrawContext &getContext() const { return mContext; }

private:
  DebugDrawContext mContext;

  void drawBox(const Transform &t, const BoxCollider &box);
  void drawCircle(const Transform &t, const CircleCollider &circle);
};

#endif // DEBUGDRAWSYSTEM_H
