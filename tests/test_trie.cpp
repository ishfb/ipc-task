#include <gtest/gtest.h>

#include <algorithm>
#include <string_view>

#include "trie.h"

namespace {
std::vector<std::string> Collect(const Trie& trie, std::string_view prefix) {
  std::vector<std::string> result;
  trie.Gather(prefix, [&](std::string_view word) { result.emplace_back(word); });
  std::ranges::sort(result);
  return result;
}
}  // namespace

TEST(TrieTest, InsertAndGather) {
  Trie trie;

  // Insert words into the trie
  trie.Insert("apple");
  trie.Insert("banana");
  trie.Insert("cherry");

  // Verify the gathered words
  const std::vector<std::string> expectedWords = {"apple"};
  EXPECT_EQ(Collect(trie, "a"), expectedWords);
}

TEST(TrieTest, PrefixSearch) {
  const Trie trie(std::vector{"a", "ab", "abc", "abcd", "bbb"});

  EXPECT_EQ(Collect(trie, ""), std::vector<std::string>({"a", "ab", "abc", "abcd", "bbb"}));
  EXPECT_EQ(Collect(trie, "a"), std::vector<std::string>({"a", "ab", "abc", "abcd"}));
  EXPECT_EQ(Collect(trie, "ab"), std::vector<std::string>({"ab", "abc", "abcd"}));
  EXPECT_EQ(Collect(trie, "abc"), std::vector<std::string>({"abc", "abcd"}));
  EXPECT_EQ(Collect(trie, "abcd"), std::vector<std::string>({"abcd"}));
  EXPECT_EQ(Collect(trie, "abcde"), std::vector<std::string>());
}

TEST(TrieTest, Remove) {
  Trie trie(std::vector{"a", "ab", "abc", "abcd", "bbb"});

  trie.Remove("ab");
  EXPECT_EQ(Collect(trie, ""), std::vector<std::string>({"a", "abc", "abcd", "bbb"}));
  EXPECT_EQ(Collect(trie, "a"), std::vector<std::string>({"a", "abc", "abcd"}));
  EXPECT_EQ(Collect(trie, "ab"), std::vector<std::string>({"abc", "abcd"}));
}

TEST(TrieTest, MoveConstructor) {
  Trie trie(std::vector{"a", "ab", "abc", "abcd", "bbb"});
  Trie newTrie(std::move(trie));

  EXPECT_EQ(Collect(newTrie, ""), std::vector<std::string>({"a", "ab", "abc", "abcd", "bbb"}));
}