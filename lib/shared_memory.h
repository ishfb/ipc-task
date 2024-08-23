#pragma once

#include <memory>
#include <span>

#include "shared_resource.h"

namespace impl
{
  struct MunmapDeleter {
    size_t size_in_bytes;

    void operator()(void* p);
  };
}  // namespace impl

class SharedMemory {
public:
  explicit SharedMemory(const SharedResource& shared_resource);

  std::span<char> Access() { return span_; }

  std::span<const char> Access() const { return span_; }

private:
  std::unique_ptr<void, impl::MunmapDeleter> data_;
  std::span<char> span_;
};
