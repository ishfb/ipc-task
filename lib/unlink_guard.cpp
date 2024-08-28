#include "unlink_guard.h"

#include <sys/mman.h>
#include <sys/stat.h>

#include "log.h"

UnlinkGuard::UnlinkGuard(std::atomic<size_t>& reference_counter, std::string name)
    : reference_counter_(reference_counter), name_(std::move(name)) {
  ++reference_counter_;
  LOG("Shared resource with name " << name_ << " reference_counter = " << reference_counter_);
}

UnlinkGuard::~UnlinkGuard() {
  if (--reference_counter_ == 0) {
    auto ret = shm_unlink(name_.c_str());
    LOG("Release shared resource with name: " << name_ << ". Ret = " << ret << ", errno = " << errno);
  } else {
    LOG("Shared resource with name " << name_ << " reference_counter = " << reference_counter_);
  }
}
