#pragma once

#include <string>

#include "string_list_view.h"

inline bool operator==(const char* lhs, const RingBufferString& rhs) {
  return lhs == std::string(rhs.begin(), rhs.end());
}

inline bool operator==(const RingBufferString& lhs, const char* rhs) {
  return std::string(lhs.begin(), lhs.end()) == rhs;
}
