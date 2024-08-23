#include <gtest/gtest.h>

#include <string>
#include <string_view>

#include "ring_buffer_allocator.h"

TEST(RingBufferAllocatorIterator, Increment) {
  std::string arena = "abcdef";
  RingBufferAllocator::Iterator it(arena.data(), 0, arena.size());

  EXPECT_EQ(*it, 'a');

  std::string letters(arena.size() * 3, ' ');
  for (auto& letter : letters) {
    letter = *it;
    ++it;
  }
  EXPECT_EQ(letters, "abcdefabcdefabcdef");
}

TEST(RingBufferAllocatorIterator, CrossBoundary) {
  std::string arena = "abcdef";
  RingBufferAllocator::Iterator begin(arena.data(), 4, arena.size());
  RingBufferAllocator::Iterator end(arena.data(), 2, arena.size());
  EXPECT_EQ(std::string(begin, end), "efab");
}

TEST(RingBufferAllocatorTest, AllocateAndDeallocate) {
  std::array<char, 16> arena;  // Example arena with 16 bytes
  RingBufferAllocator allocator(arena);

  // Allocate 8 bytes
  auto range1 = allocator.Allocate(8);
  ASSERT_TRUE(range1.has_value());
  EXPECT_EQ(range1.value().size(), 8);

  // Allocate 4 bytes, total 12 bytes allocated
  auto range2 = allocator.Allocate(4);
  ASSERT_TRUE(range2.has_value());
  EXPECT_EQ(range2.value().size(), 4);

  // Try to allocate 10 bytes (not enough space)
  {
    auto saved_begin = allocator.begin();
    auto saved_end = allocator.end();
    auto range3 = allocator.Allocate(10);
    EXPECT_FALSE(range3.has_value());
    EXPECT_EQ(allocator.begin(), saved_begin);
    EXPECT_EQ(allocator.end(), saved_end);
  }
  // Deallocate the first allocation, total 4 bytes allocated
  allocator.Deallocate(range1.value().size());

  // Allocate 6 bytes (should fit in the previously deallocated space)
  // total 10 bytes allocated
  auto range4 = allocator.Allocate(6);
  ASSERT_TRUE(range4.has_value());
  EXPECT_EQ(range4.value().size(), 6);

  for (size_t bytes : {6, 7, 12}) {
    auto range5 = allocator.Allocate(bytes);
    EXPECT_FALSE(range5.has_value());
  }
  EXPECT_TRUE(allocator.Allocate(5).has_value());
}

TEST(RingBufferAllocatorTest, Capacity) {
  std::array<char, 19> arena;
  RingBufferAllocator allocator(arena);

  EXPECT_EQ(allocator.Capacity(), 18);

  allocator.Allocate(10);

  EXPECT_EQ(allocator.Capacity(), 18);
  allocator.Deallocate(8);
  EXPECT_EQ(allocator.Capacity(), 18);
}

TEST(RingBufferAllocatorTest, FullAndEmpty) {
  std::array<char, 27> arena;
  RingBufferAllocator allocator(arena);

  EXPECT_EQ(allocator.begin(), allocator.end());
  allocator.Allocate(25);
  allocator.Deallocate(23);
  allocator.Allocate(24);
  EXPECT_NE(allocator.begin(), allocator.end());
  EXPECT_FALSE(allocator.Allocate(1).has_value());
}