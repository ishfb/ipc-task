#include "ring_buffer_allocator.h"

RingBufferAllocator::RingBufferAllocator(std::span<char> arena)
    : arena_(arena), begin_(arena_.data(), 0, arena_.size()), end_(begin_) {}

std::optional<Range<RingBufferAllocator::Iterator>> RingBufferAllocator::Allocate(size_t bytes) {
  if (bytes > Capacity() - allocated_bytes_) return std::nullopt;

  allocated_bytes_ += bytes;
  auto new_end = std::next(end_, bytes);
  Range ret(end_, new_end);
  new_end = end_;
  return ret;
}

void RingBufferAllocator::Deallocate(size_t bytes) {
  bytes = std::min(bytes, allocated_bytes_);
  allocated_bytes_ -= bytes;
  begin_ = std::next(begin_, bytes);
}
