#include <gtest/gtest.h>

#include <string_view>

#include "trie.h"

namespace {
template <typename T>
std::vector<std::string_view> Collect(const Trie<T>& trie, std::string_view prefix) {
  std::vector<std::string_view> result;
  trie.Gather(prefix, [&](std::string_view word) { result.push_back(word); });
  return result;
}
}  // namespace

TEST(TrieTest, InsertAndGather) {
  Trie<std::string_view> trie;

  // Insert words into the trie
  trie.Insert("apple");
  trie.Insert("banana");
  trie.Insert("cherry");

  // Gather words with a specific prefix
  std::vector<std::string> gatheredWords;
  trie.Gather("a", [&](std::string_view word) { gatheredWords.push_back(std::string(word)); });

  // Verify the gathered words
  const std::vector<std::string> expectedWords = {"apple"};
  ASSERT_EQ(gatheredWords, expectedWords);
}

TEST(TrieTest, PrefixSearch) {
  const Trie<std::string> trie(std::vector{"a", "ab", "abc", "abcd", "bbb"});

  EXPECT_EQ(Collect(trie, ""), std::vector<std::string_view>({"a", "ab", "abc", "abcd", "bbb"}));
  EXPECT_EQ(Collect(trie, "a"), std::vector<std::string_view>({"a", "ab", "abc", "abcd"}));
  EXPECT_EQ(Collect(trie, "ab"), std::vector<std::string_view>({"ab", "abc", "abcd"}));
  EXPECT_EQ(Collect(trie, "abc"), std::vector<std::string_view>({"abc", "abcd"}));
  EXPECT_EQ(Collect(trie, "abcd"), std::vector<std::string_view>({"abcd"}));
  EXPECT_EQ(Collect(trie, "abcde"), std::vector<std::string_view>());
}

TEST(TrieTest, Remove) {
  Trie<std::string> trie(std::vector{"a", "ab", "abc", "abcd", "bbb"});

  trie.Remove("ab");
  EXPECT_EQ(Collect(trie, ""), std::vector<std::string_view>({"a", "abc", "abcd", "bbb"}));
  EXPECT_EQ(Collect(trie, "a"), std::vector<std::string_view>({"a", "abc", "abcd"}));
  EXPECT_EQ(Collect(trie, "ab"), std::vector<std::string_view>({"abc", "abcd"}));
}

TEST(TrieTest, MoveConstructor) {
  Trie<std::string> trie(std::vector{"a", "ab", "abc", "abcd", "bbb"});
  Trie<std::string> newTrie(std::move(trie));

  EXPECT_EQ(Collect(newTrie, ""), std::vector<std::string_view>({"a", "ab", "abc", "abcd", "bbb"}));
}