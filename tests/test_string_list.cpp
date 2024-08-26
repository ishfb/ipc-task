#include <gtest/gtest.h>

#include <sstream>

#include "string_list.h"
#include "string_list_view_test_helper.h"

TEST(StringListTest, Add) {
  std::array<char, 270> arena;
  StringList string_list(arena);

  string_list.Add("hello");
  string_list.Add("world");
  string_list.Add("let's");
  string_list.Add("play");

  EXPECT_EQ(string_list.back(), "play");

  std::ostringstream out;
  for (const auto& s : string_list) {
    out << s << ' ';
  }
  EXPECT_EQ(out.str(), "hello world let's play ");
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
  std::array<char, 42> arena;
  StringList string_list(arena);

  string_list.Add("hello");
  string_list.Add("world");
  string_list.Add("let's");

  EXPECT_EQ(string_list.back(), "let's");

  {
    std::ostringstream out;
    for (const auto& s : string_list) {
      out << s << ' ';
    }
    EXPECT_EQ(out.str(), "world let's ");
  }
  string_list.Add("hellohelloh");
  EXPECT_EQ(string_list.back(), "hellohelloh");

  {
    std::ostringstream out;
    for (const auto& s : string_list) {
      out << s << ' ';
    }
    EXPECT_EQ(out.str(), "hellohelloh ");
  }
}
