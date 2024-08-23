#pragma once

#include <optional>
#include <span>

class RingBufferAllocator {
public:
  explicit RingBufferAllocator(std::span<std::byte> arena);

  std::optional<std::span<std::byte>> Allocate(size_t bytes);
  void Deallocate(std::span<std::byte> region);
};