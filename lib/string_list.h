#pragma once

#include <span>
#include <string_view>

#include "string_list_view.h"

class StringList : public StringListView {
public:
  explicit StringList(std::span<char> arena);

  void Add(std::string_view new_string);
};
