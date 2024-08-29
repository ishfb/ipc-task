#pragma once

#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "log.h"

template <typename T>
concept StringLike = requires(const T& t) {
  std::begin(t);
  std::end(t);
  std::constructible_from<std::string, decltype(std::begin(t)), decltype(std::end(t))>;
};

class Trie {
public:
  Trie() : nodes_(1) {}

  template <typename Container>
    requires requires(Container& c) {
      std::begin(c);
      std::end(c);
    }
  explicit Trie(const Container& words) : Trie() {
    for (auto&& w : words) {
      Insert(w);
    }
  }

  void Insert(const char* word) { Insert(std::string_view(word)); }

  void Insert(const StringLike auto& word) {
    size_t node = 0;
    for (char c : word) {
      if (!nodes_[node].children_.contains(c)) {
        nodes_[node].children_[c] = nodes_.size();
        nodes_.emplace_back();
      }
      node = nodes_[node].children_[c];
    }
    nodes_[node].terminal_ = true;
  }

  void Remove(const char* word) { Remove(std::string_view(word)); }

  void Remove(const StringLike auto& word) {
    size_t node = 0;
    for (char c : word) {
      if (!nodes_[node].children_.contains(c)) {
        return;
      }
      node = nodes_[node].children_[c];
    }
    nodes_[node].terminal_ = false;
  }

  void Gather(const StringLike auto& prefix, std::regular_invocable<std::string_view> auto callback) const {
    LOG("Start gather for prefix " << prefix);
    size_t node = 0;
    for (char c : prefix) {
      auto& children = nodes_[node].children_;
      if (auto it = children.find(c); it == children.end()) {
        // LOG("No subtree found");
        return;
      } else {
        node = it->second;
      }
    }

    std::string str(prefix);
    DoGather(node, str, callback);
  }

private:
  void DoGather(size_t node, std::string& cur_prefix, std::regular_invocable<std::string_view> auto callback) const {
    // LOG("DoGather, prefix = " << cur_prefix);
    if (nodes_[node].terminal_) {
      LOG("DoGather — match with " << cur_prefix);
      callback(cur_prefix);
    }

    for (const auto& [c, child] : nodes_[node].children_) {
      cur_prefix.push_back(c);
      DoGather(child, cur_prefix, callback);
      cur_prefix.pop_back();
    }
  }

  struct Node {
    bool terminal_ = false;
    std::unordered_map<char, size_t> children_;
  };

  std::vector<Node> nodes_;
};
