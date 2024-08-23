#include "ring_buffer_allocator.h"

RingBufferAllocator::RingBufferAllocator(std::span<char> arena) {}

std::optional<Range<RingBufferAllocator::iterator>> RingBufferAllocator::Allocate(size_t bytes) { return std::nullopt; }

void RingBufferAllocator::Deallocate(Range<RingBufferAllocator::iterator> region) {}
