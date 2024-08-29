#pragma once

#include <ranges>
#include <string_view>
#include <type_traits>

template <typename T>
  // requires(std::ranges::range<T> &&                            // T must be a range (i.e., iterable)
  //          std::same_as<std::ranges::range_value_t<T>, char>)  // The value type of the range must be char
class Trie {
public:
  Trie() {} 

  template <typename Container>
  // requires(std::ranges::range<Container> &&                       
  //          std::same_as<std::ranges::range_value_t<Container>, T>)
  Trie(const Container& words) {}

  void Insert(const T& word) {}
  void Remove(const T& word) {}

  template <typename Callback>
  void Gather(std::string_view prefix, Callback callback) const {}
};