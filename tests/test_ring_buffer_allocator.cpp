#include <gtest/gtest.h>

#include <string_view>
#include <string>

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

  // Allocate 4 bytes
  auto range2 = allocator.Allocate(4);
  ASSERT_TRUE(range2.has_value());
  EXPECT_EQ(range2.value().size(), 4);

  // Try to allocate 10 bytes (not enough space)
  auto range3 = allocator.Allocate(10);
  EXPECT_FALSE(range3.has_value());

  // Deallocate the first allocation
  allocator.Deallocate(range1.value().size());

  // Allocate 6 bytes (should fit in the previously deallocated space)
  auto range4 = allocator.Allocate(6);
  ASSERT_TRUE(range4.has_value());
  EXPECT_EQ(range4.value().size(), 6);

  // Try to allocate 12 bytes (not enough space)
  auto range5 = allocator.Allocate(12);
  EXPECT_FALSE(range5.has_value());
}

TEST(RingBufferAllocatorTest, AllocateZeroBytes) {
  std::array<char, 16> arena;  // Example arena with 16 bytes
  RingBufferAllocator allocator(arena);

  // Try to allocate 0 bytes
  auto range = allocator.Allocate(0);
  EXPECT_FALSE(range.has_value());
}

TEST(RingBufferAllocatorTest, Capacity) {
  std::array<char, 19> arena;  // Example arena with 16 bytes
  RingBufferAllocator allocator(arena);

  EXPECT_EQ(allocator.Capacity(), 19);

  allocator.Allocate(10);

  EXPECT_EQ(allocator.Capacity(), 19);
  allocator.Deallocate(8);
  EXPECT_EQ(allocator.Capacity(), 19);
}