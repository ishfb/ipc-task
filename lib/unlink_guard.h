#pragma once

#include <atomic>
#include <string>

class UnlinkGuard {
public:
  explicit UnlinkGuard(std::atomic<size_t>& reference_counter, std::string name);
  ~UnlinkGuard();

private:
  std::atomic<size_t>& reference_counter_;
  std::string name_;
};
