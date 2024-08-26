#include <gtest/gtest.h>

#include "string_list_view.h"

bool operator==(const char* lhs, const RingBufferString& rhs) { return lhs == std::string(rhs.begin(), rhs.end()); }
bool operator==(const RingBufferString& lhs, const char* rhs) { return std::string(lhs.begin(), lhs.end()) == rhs; }

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