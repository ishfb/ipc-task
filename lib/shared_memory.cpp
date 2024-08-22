#include "shared_memory.h"

#include <sys/mman.h>

namespace impl {
void MunmapDeleter::operator()(void* p) { munmap(p, this->size_in_bytes); }
}  // namespace impl

SharedMemory::SharedMemory(const SharedResource& shared_resource)
    : data_(mmap(nullptr, shared_resource.SizeInBytes(), PROT_WRITE | PROT_READ, MAP_SHARED, shared_resource.Handle(), 0),
            impl::MunmapDeleter{shared_resource.SizeInBytes()}),
      span_(static_cast<std::byte*>(data_.get()), shared_resource.SizeInBytes()) {}
