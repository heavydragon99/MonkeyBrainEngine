#ifndef ARCHETYPE_H
#define ARCHETYPE_H

#include <bitset>
#include <vector>

constexpr size_t MAX_COMPONENTS = 32;
using ComponentMask = std::bitset<MAX_COMPONENTS>;

struct Archetype {
  ComponentMask mask;
  std::vector<void *> chunks; // store Chunk pointers
};

#endif // ARCHETYPE_H
