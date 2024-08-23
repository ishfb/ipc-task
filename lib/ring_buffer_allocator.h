#pragma once

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

class LinearAllocator  {
  public:
  explicit LinearAllocator(std::span<char> arena) {}

  template<typename T>
  T* AllocateFor(T*) {}

  std::span<char> FreeSpace() const {}
};