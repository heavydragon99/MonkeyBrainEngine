#ifndef SCRIPTSSYSTEM_H
#define SCRIPTSSYSTEM_H

#include "system/system.h"

class ScriptsSystem final : public System {
public:
  bool initialize() override;
  void update(Registry &aRegistry, float aDt) override;
};

#endif // SCRIPTSSYSTEM_H
