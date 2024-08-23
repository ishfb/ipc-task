#include <gtest/gtest.h>

#include <array>

#include "ring_buffer_allocator.h"

TEST(LinearAllocator, Construction) {
  std::array<char, 1024> arena;
  LinearAllocator allocator(arena);
  EXPECT_EQ(allocator.FreeSpace().size(), 1024);
}

TEST(LinearAllocator, AllocateFor) {
  std::array<char, 1024> arena;
  LinearAllocator allocator(arena);

  int* ptr;
  ptr = allocator.AllocateFor(ptr);
  EXPECT_EQ(allocator.FreeSpace().size(), 1024 - sizeof(int));
  EXPECT_NE(ptr, nullptr);
}

TEST(LinearAllocator, AllocateForNotEnoughSpace) {
  std::array<char, 4> arena;
  LinearAllocator allocator(arena);

  auto* ptr = allocator.AllocateFor<uint64_t>(nullptr);
  EXPECT_EQ(allocator.FreeSpace().size(), 4);
  EXPECT_EQ(ptr, nullptr);
}

