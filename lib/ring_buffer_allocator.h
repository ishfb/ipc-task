#pragma once

#include <cstdint>
#include <optional>
#include <span>
#include <tuple>

template <typename Iter>
class Range {
public:
  Range(Iter begin, Iter end) : begin_(begin), end_(end) {}

  auto begin() { return begin_; }
  auto end() { return end_; }
  auto begin() const { return begin_; }
  auto end() const { return end_; }

  auto size() const { return std::distance(begin_, end_); }

private:
  Iter begin_, end_;
};

class RingBufferAllocator {
public:
  explicit RingBufferAllocator(std::span<char> arena);

  struct Iterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = char;
    using pointer = char*;
    using reference = char&;

    Iterator(pointer base, size_t index, size_t size) : base_(base), index_(index), size_(size) {}

    reference operator*() const { return base_[index_]; }
    pointer operator->() { return base_ + index_; }
    Iterator& operator++() {
      index_ = (index_ + 1) % size_;
      return *this;
    }
    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }
    friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
      return std::tie(lhs.base_, lhs.index_, lhs.size_) == std::tie(rhs.base_, rhs.index_, rhs.size_);
    };
    friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
      return std::tie(lhs.base_, lhs.index_, lhs.size_) != std::tie(rhs.base_, rhs.index_, rhs.size_);
    };

  private:
    pointer base_;
    size_t index_;
    size_t size_;
  };

  Iterator begin() { return begin_; }
  Iterator end() { return end_; }

  size_t Capacity() const { return arena_.size(); }

  // Tries to allocate from the and of the ring buffer
  std::optional<Range<Iterator>> Allocate(size_t bytes);
  // Deallocates the first `bytes` bytes from the beginning of the ring buffer
  void Deallocate(size_t bytes);

private:
  std::span<char> arena_;
  Iterator begin_, end_;
  size_t allocated_bytes_ = 0;
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