#include "shared_resource.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/unistd.h>

#include "log.h"

const std::string kSharedResourceDefaultName = "/ipc_task_shared_resource";

SharedResource::SharedResource(size_t owner_count, size_t size_in_bytes, const std::string& name)
    : name_(name.empty() ? kSharedResourceDefaultName : name),
      handle_(shm_open(name_.c_str(), O_CREAT | O_RDWR, S_IRWXU)),
      size_in_bytes_(size_in_bytes) {
  LOG("Created shared resource with name: " << name_);
  auto ret = ftruncate(handle_, size_in_bytes);
  LOG("ftruncate ret = " << ret << ", errno = " << errno);
}

SharedResource::~SharedResource() {
  close(handle_);
}
