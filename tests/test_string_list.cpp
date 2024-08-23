#include <gtest/gtest.h>

#include <sstream>

#include "string_list.h"

TEST(StringListTest, RingBufferStringSerialization) {
  std::string arena = "abcdef";
  RingBufferString s({arena.data(), 4, arena.size()}, {arena.data(), 1, arena.size()});

  std::ostringstream out;
  out << s;

  EXPECT_EQ(out.str(), "efa");
}

TEST(StringListTest, AddAndView) {
  std::array<char, 270> arena;
  StringList string_list(arena);
  StringListView view(arena);

  string_list.Add("hello");
  string_list.Add("world");
  string_list.Add("let's");
  string_list.Add("play");

  EXPECT_EQ(view.back(), "play");

  std::ostringstream out;
  for (const auto& s : view) {
    out << s << ' ';
  }
  EXPECT_EQ(out.str(), "hello world let's play ");
}

TEST(StringListTest, AddNotEnoughSpace) {
  std::array<char, 34> arena;
  StringList string_list(arena);
  StringListView view(arena);

  string_list.Add("hello");
  string_list.Add("world");
  string_list.Add("let's");

  EXPECT_EQ(view.back(), "let's");

  std::ostringstream out;
  for (const auto& s : view) {
    out << s << ' ';
  }
  EXPECT_EQ(out.str(), "world let's ");
}
