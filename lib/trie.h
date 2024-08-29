#pragma once

#include <string_view>
#include <type_traits>

template <typename T>
concept StringLike = requires(const T& t) {
  std::begin(t);
  std::end(t);
  std::constructible_from<std::string, decltype(std::begin(t)), decltype(std::end(t))>;
};

class Trie {
public:
  Trie() {}

  template <typename Container>
    requires requires(Container& c) {
      std::begin(c);
      std::end(c);
    }
  explicit Trie(const Container& words) {}

  void Insert(const StringLike auto& word) {}
  void Remove(const StringLike auto& word) {}

  void Gather(const StringLike auto& prefix, std::regular_invocable<std::string_view> auto callback) const {}
};
