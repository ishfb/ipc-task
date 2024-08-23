#include "ring_buffer_allocator.h"

RingBufferAllocator::RingBufferAllocator(std::span<char> arena)
    : arena_(arena), begin_(arena_.data(), 0, arena_.size()), end_(begin_) {}

std::optional<Range<RingBufferAllocator::Iterator>> RingBufferAllocator::Allocate(size_t bytes) { return std::nullopt; }

void RingBufferAllocator::Deallocate(size_t bytes) {}
