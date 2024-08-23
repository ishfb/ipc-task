#pragma once

#include <cstdint>
#include <optional>
#include <span>

template <typename Iter>
class Range {
public:
  Range(Iter begin, Iter end) : begin_(begin), end_(end) {}

  auto begin() { return begin_; }
  auto end() { return end_; }
  auto begin() const { return begin_; }
  auto end() const { return end_; }

private:
  Iter begin_, end_;
};

class RingBufferAllocator {
public:
  explicit RingBufferAllocator(std::span<char> arena);

  using iterator = char*;

  iterator begin() {}
  iterator end() {}

  size_t Capacity() const {}

  std::optional<Range<iterator>> Allocate(size_t bytes);
  void Deallocate(Range<iterator> region);
};

class LinearAllocator {
public:
  explicit LinearAllocator(std::span<char> arena) : arena_(arena) {}

  template <typename T>
  T* AllocateFor(T*) {
    if (arena_.size() < sizeof(T)) {
      return nullptr;
    }

    auto alignment = alignof(T);
    auto offset = (alignment - (reinterpret_cast<uintptr_t>(arena_.data()) % alignment)) % alignment;
    if (arena_.size() < sizeof(T) + offset) {
      return nullptr;
    }

    auto ptr = reinterpret_cast<T*>(arena_.data() + offset);
    arena_ = arena_.subspan(sizeof(T) + offset);
    return ptr;
  }

  std::span<char> FreeSpace() const { return arena_; }

private:
  std::span<char> arena_;
};