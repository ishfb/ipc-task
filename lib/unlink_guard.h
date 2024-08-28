#pragma once

#include <string>

class UnlinkGuard {
public:
  explicit UnlinkGuard(size_t& reference_counter, std::string name);
  ~UnlinkGuard();

private:
  size_t& reference_counter_;
  std::string name_;
};
