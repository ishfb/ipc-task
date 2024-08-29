#include <gtest/gtest.h>

#include <string_view>

#include "trie.h"

namespace {
std::vector<std::string_view> Collect(const Trie& trie, std::string_view prefix) {
  std::vector<std::string_view> result;
  trie.Gather(prefix, [&](std::string_view word) { result.push_back(word); });
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
  const std::vector<std::string_view> expectedWords = {"apple"};
  EXPECT_EQ(Collect(trie, "a"), expectedWords);
}

TEST(TrieTest, PrefixSearch) {
  const Trie trie(std::vector{"a", "ab", "abc", "abcd", "bbb"});

  EXPECT_EQ(Collect(trie, ""), std::vector<std::string_view>({"a", "ab", "abc", "abcd", "bbb"}));
  EXPECT_EQ(Collect(trie, "a"), std::vector<std::string_view>({"a", "ab", "abc", "abcd"}));
  EXPECT_EQ(Collect(trie, "ab"), std::vector<std::string_view>({"ab", "abc", "abcd"}));
  EXPECT_EQ(Collect(trie, "abc"), std::vector<std::string_view>({"abc", "abcd"}));
  EXPECT_EQ(Collect(trie, "abcd"), std::vector<std::string_view>({"abcd"}));
  EXPECT_EQ(Collect(trie, "abcde"), std::vector<std::string_view>());
}

TEST(TrieTest, Remove) {
  Trie trie(std::vector{"a", "ab", "abc", "abcd", "bbb"});

  trie.Remove("ab");
  EXPECT_EQ(Collect(trie, ""), std::vector<std::string_view>({"a", "abc", "abcd", "bbb"}));
  EXPECT_EQ(Collect(trie, "a"), std::vector<std::string_view>({"a", "abc", "abcd"}));
  EXPECT_EQ(Collect(trie, "ab"), std::vector<std::string_view>({"abc", "abcd"}));
}

TEST(TrieTest, MoveConstructor) {
  Trie trie(std::vector{"a", "ab", "abc", "abcd", "bbb"});
  Trie newTrie(std::move(trie));

  EXPECT_EQ(Collect(newTrie, ""), std::vector<std::string_view>({"a", "ab", "abc", "abcd", "bbb"}));
}