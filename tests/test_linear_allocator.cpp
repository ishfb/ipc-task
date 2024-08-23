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

TEST(LinearAllocator, AllocateForProperAlignment) {
  std::array<char, 1024> arena;
  LinearAllocator allocator(arena);

  // Allocate memory for a double, which requires 8-byte alignment
  double* ptr = allocator.AllocateFor<double>(nullptr);
  EXPECT_EQ(allocator.FreeSpace().size(), 1024 - sizeof(double));
  EXPECT_NE(ptr, nullptr);
  EXPECT_EQ(reinterpret_cast<uintptr_t>(ptr) % alignof(double), 0);

  // Allocate memory for an int, which requires 4-byte alignment
  int* ptr2 = allocator.AllocateFor<int>(nullptr);
  EXPECT_EQ(allocator.FreeSpace().size(), 1024 - sizeof(double) - sizeof(int));
  EXPECT_NE(ptr2, nullptr);
  EXPECT_EQ(reinterpret_cast<uintptr_t>(ptr2) % alignof(int), 0);
}
