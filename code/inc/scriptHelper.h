#ifndef SCRIPTHELPER_H
#define SCRIPTHELPER_H

#include "components.h"
#include <type_traits>

template <typename T, typename... Args>
void addScript(Registry &registry, Entity entity, Args &&...args) {
  static_assert(std::is_base_of_v<Script, T>, "T must derive from Script");
  registry.add(entity, ScriptComponent(
                           std::make_unique<T>(std::forward<Args>(args)...)));
}

#endif // SCRIPTHELPER_H
