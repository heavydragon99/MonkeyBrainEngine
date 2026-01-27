#include "scripts/scriptsSystem.h"

#include "components.h"

bool ScriptsSystem::initialize() { return true; }

void ScriptsSystem::update(Registry &aRegistry, float aDt) {
  aRegistry.forEach<ScriptComponent>(
      [&](Entity aEntity, ScriptComponent &aScriptComp) {
        if (!aScriptComp.enabled || !aScriptComp.instance)
          return;

        if (!aScriptComp.initialized) {
          aScriptComp.instance->onCreate(aEntity, aRegistry);
          aScriptComp.initialized = true;
        }

        aScriptComp.instance->onUpdate(aEntity, aRegistry, aDt);
      });
}
