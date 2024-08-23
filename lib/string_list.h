#pragma once

#include <ostream>
#include <span>
#include <string_view>

#include "ring_buffer_allocator.h"

struct RingBufferString : public Range<RingBufferAllocator::Iterator> {
  using Range<RingBufferAllocator::Iterator>::Range;
};

std::ostream& operator<<(std::ostream& lhs, const RingBufferString& rhs);

class StringListBase {
public:
  struct iterator {
    RingBufferAllocator::Iterator first, last;

    iterator& operator++() {}
    bool operator==(iterator) {}
    RingBufferString operator*() const { return {first, last}; }
    
  };

protected:
  StringListBase(std::span<char> arena);

protected:
  LinearAllocator nodes_allocator_;
  iterator* begin_;
  iterator* end_;
  iterator* back_;
};

class StringList : public StringListBase {
public:
  explicit StringList(std::span<char> arena);
  ~StringList() {}

  void Add(std::string_view new_string);

private:
  RingBufferAllocator string_allocator_;
};

class StringListView : public StringListBase {
public:
  explicit StringListView(std::span<char> arena) : StringListBase(arena) {}

  const iterator begin() const { return *begin_; }
  const iterator end() const { return *end_; }
  std::string back() const { return {back_->first, back_->last}; }
};