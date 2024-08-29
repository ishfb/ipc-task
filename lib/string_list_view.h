#pragma once

#include <cstddef>
#include <ostream>
#include <ranges>

#include "ring_buffer_allocator.h"

struct RingBufferString : public Range<RingBufferAllocator::ConstIterator> {
  using Range<RingBufferAllocator::ConstIterator>::Range;
};

std::ostream& operator<<(std::ostream& lhs, const RingBufferString& rhs);

class StringListView {
public:
  struct ConstIterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = RingBufferString;
    using pointer = RingBufferString*;
    using reference = RingBufferString&;

    explicit ConstIterator(RingBufferAllocator::ConstIterator base) : base_(base) {}

    RingBufferString operator*() const { return {base_, FindStringEnd(base_)}; }
    ConstIterator& operator++() {
      base_ = FindStringEnd(base_);
      ++base_;
      return *this;
    }
    ConstIterator operator++(int) {
      ConstIterator tmp = *this;
      ++(*this);
      return tmp;
    }
    friend bool operator==(const ConstIterator& lhs, const ConstIterator& rhs) { return lhs.base_ == rhs.base_; };
    friend bool operator!=(const ConstIterator& lhs, const ConstIterator& rhs) { return lhs.base_ != rhs.base_; };

    RingBufferAllocator::ConstIterator Base() const { return base_; } 

  private:
    RingBufferAllocator::ConstIterator base_;

    static RingBufferAllocator::ConstIterator FindStringEnd(RingBufferAllocator::ConstIterator cur) {
      for (; *cur; ++cur) {
      }
      return cur;
    }
  };

  explicit StringListView(std::span<char> arena);

  ConstIterator begin() const { return ConstIterator{string_allocator_.begin().WithIndex(begin_)}; }
  ConstIterator end() const { return ConstIterator{string_allocator_.begin().WithIndex(end_)}; }

  RingBufferString back() const {
    ConstIterator back{string_allocator_.begin().WithIndex(back_)};
    return *back;
  }

  bool empty() const { return begin_ == end_; }

protected:
  LinearAllocator nodes_allocator_;
  volatile size_t& begin_;
  volatile size_t& end_;
  volatile size_t& back_;
  RingBufferAllocator string_allocator_;
};
