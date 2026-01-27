// #ifndef ECSREGISTRY_H
// #define ECSREGISTRY_H
//
// #include "archetype.h"
// #include "chunk.h"
// #include "components.h"
// #include "entity.h"

// #include <memory_resource>
// #include <tuple>
// #include <unordered_map>
//
// class Registry {
// public:
//   Registry() : arena(1024 * 1024) {} // 1MB arena
//
//   Entity createEntity() { return ++nextEntity; }
//
//   // Add components to entity, supports incremental addition
//   template <typename... Components>
//   void add(Entity aEntity, Components... aComponents) {
//     ComponentMask mask;
//     (mask.set(ComponentTypeID<Components>::id()), ...);
//
//     // Merge with existing components if entity exists
//     if (entityToArchetype.contains(aEntity)) {
//       mask |= entityToArchetype[aEntity]->mask;
//     }
//
//     // Get or create archetype
//     Archetype *arch = getOrCreateArchetype<Components...>(mask);
//
//     // Get chunk
//     Chunk<Components...> *chunk;
//     if (arch->chunks.empty()) {
//       auto alloc =
//           std::pmr::polymorphic_allocator<Chunk<Components...>>(&arena);
//       chunk = alloc.allocate(1);
//       alloc.construct(chunk, Chunk<Components...>(&arena));
//       arch->chunks.push_back(chunk);
//     } else {
//       chunk = static_cast<Chunk<Components...> *>(arch->chunks[0]);
//     }
//
//     chunk->add(std::make_tuple(aComponents...));
//     entityToArchetype[aEntity] = arch;
//   }
//
//   template <typename... Components, typename Func> void forEach(Func &&aFunc)
//   {
//     ComponentMask mask;
//     (mask.set(ComponentTypeID<Components>::id()), ...);
//
//     for (auto &[archMask, archList] : archetypes) {
//       if ((archMask & mask) == mask) {
//         for (auto *archPtr : archList) {
//           auto *arch = static_cast<Archetype *>(archPtr);
//           for (auto *chunkPtr : arch->chunks) {
//             auto *chunk = static_cast<Chunk<Components...> *>(chunkPtr);
//             for (auto &tpl : chunk->data) {
//               std::apply(aFunc, tpl);
//             }
//           }
//         }
//       }
//     }
//   }
//
// private:
//   Entity nextEntity = 0;
//   std::pmr::monotonic_buffer_resource arena;
//   std::unordered_map<Entity, Archetype *> entityToArchetype;
//   std::unordered_map<ComponentMask, std::vector<void *>> archetypes;
//
//   template <typename... Components>
//   Archetype *getOrCreateArchetype(ComponentMask mask) {
//     auto it = archetypes.find(mask);
//     if (it != archetypes.end() && !it->second.empty())
//       return static_cast<Archetype *>(it->second[0]);
//
//     // Construct archetype in arena
//     std::pmr::polymorphic_allocator<Archetype> archAlloc(&arena);
//     Archetype *arch = archAlloc.allocate(1);
//     archAlloc.construct(arch, Archetype{});
//     arch->mask = mask;
//
//     archetypes[mask].push_back(reinterpret_cast<void *>(arch));
//     return arch;
//   }
// };
//
// #endif // ECSREGISTRY_H

#ifndef ECSREGISTRY_H
#define ECSREGISTRY_H

#include <iostream>
#include <memory>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <unordered_map>

#include "entity.h"

class Registry {
public:
  Registry() = default;

  // ---------------- Entities ----------------
  Entity createEntity() { return ++mNextEntity; }

  // ---------------- Add Components ----------------

  template <typename T> void add(Entity entity, T &&component) {
    getStorage<std::decay_t<T>>().emplace(entity, std::forward<T>(component));
    std::cout << "Added component of type " << typeid(T).name() << " to entity "
              << entity << "\n";
  }

  // ---------------- Get Components ----------------
  template <typename T> T *get(Entity entity) {
    auto &storage = getStorage<T>();
    auto it = storage.find(entity);
    return (it != storage.end()) ? &it->second : nullptr;
  }

  // ---------------- Has Components ----------------
  template <typename T> bool has(Entity entity) {
    auto &storage = getStorage<T>();
    return storage.contains(entity);
  }

  // ---------------- forEach ----------------
  template <typename... Components, typename Func> void forEach(Func &&func) {
    using First = std::tuple_element_t<0, std::tuple<Components...>>;
    auto &primary = getStorage<First>();

    for (auto &[entity, firstComponent] : primary) {

      if ((has<Components>(entity) && ...)) {
        func(entity, *get<Components>(entity)...);
      }
    }
  }

private:
  Entity mNextEntity = 0;

  // ---------------- Storage ----------------
  struct IStorage {
    virtual ~IStorage() = default;
  };

  template <typename T> struct Storage : IStorage {
    std::unordered_map<Entity, T> data;
  };

  std::unordered_map<std::type_index, std::unique_ptr<IStorage>> mStorages;

  template <typename T> std::unordered_map<Entity, T> &getStorage() {
    auto type = std::type_index(typeid(T));

    auto it = mStorages.find(type);
    if (it == mStorages.end()) {
      auto storage = std::make_unique<Storage<T>>();
      auto *ptr = storage.get();
      mStorages[type] = std::move(storage);
      return ptr->data;
    }

    return static_cast<Storage<T> *>(it->second.get())->data;
  }
};

#endif // ECSREGISTRY_H
