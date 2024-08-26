#include <gtest/gtest.h>

#include "string_list_view.h"
#include "string_list_view_test_helper.h"

TEST(RingBufferStringTest, Serialization) {
  std::string arena = "abcdef";
  RingBufferString s({arena.data(), 4, arena.size()}, {arena.data(), 1, arena.size()});

  std::ostringstream out;
  out << s;

  EXPECT_EQ(out.str(), "efa");
}

class StringListViewTest : public ::testing::Test {
protected:
  void SetUp() override {
    const char strings[] = "ree\0     One\0Two\0Th";
    const size_t indexes[] = {9, 4, 17};

    arena_.resize(sizeof(indexes) + sizeof(strings) - 1);
    auto i = std::copy((char*)indexes, (char*)indexes + sizeof(indexes), arena_.begin());
    std::copy(strings, strings + sizeof(strings) - 1, i);
  }

  std::vector<char> arena_;
};

TEST_F(StringListViewTest, ConstructFromArena) {
  StringListView view(arena_);

  EXPECT_EQ(*view.begin(), "One");
  EXPECT_EQ(view.back(), "Three");
}

TEST_F(StringListViewTest, Iteration) {
  StringListView view(arena_);

  std::ostringstream out;
  for (const auto& s : view) {
    out << s << ' ';
  }
  EXPECT_EQ(out.str(), "One Two Three ");
}