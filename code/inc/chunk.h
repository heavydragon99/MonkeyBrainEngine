#ifndef CHUNK_H
#define CHUNK_H

#include <memory_resource>
#include <tuple>
#include <vector>

template <typename... Components> struct Chunk {
  std::pmr::vector<std::tuple<Components...>> data;

  Chunk(std::pmr::memory_resource *aMr) : data(aMr) {}

  void add(std::tuple<Components...> aTpl) { data.push_back(std::move(aTpl)); }

  size_t size() const { return data.size(); }
};

#endif // CHUNK_H
