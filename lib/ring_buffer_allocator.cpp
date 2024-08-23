#include "ring_buffer_allocator.h"

RingBufferAllocator::RingBufferAllocator(std::span<std::byte> arena) {}

std::optional<std::span<std::byte>> RingBufferAllocator::Allocate(size_t bytes) {
  return std::optional<std::span<std::byte>>();
}

void RingBufferAllocator::Deallocate(std::span<std::byte> region) {}
