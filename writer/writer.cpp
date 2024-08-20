#include <sys/mman.h>
#include <unistd.h>

#include <iostream>
#include <span>
#include <string>

class SharedResource {
public:
  SharedResource(std::string name = "") {}
  ~SharedResource() {}

  using ShmId = int;
  ShmId Handle() const { return handle_; }
  const std::string& Name() const { return name_; }

  void Cleanup() {}

private:
  std::string name_;
  ShmId handle_;
};

class SharedMemory {
public:
  explicit SharedMemory(
      const SharedResource& shared_resource, size_t bytes) {
  }
  ~SharedMemory() {}

  std::span<std::byte> Access() {}
};

template <typename AtEnter, typename AtExit>
class SemaphoreGuard {
public:
  explicit SemaphoreGuard(size_t count, AtEnter at_enter,
                          AtExit at_exit)
      : count_(count),
        at_enter_(at_enter),
        at_exit_(at_exit) {}

  ~SemaphoreGuard() {}

private:
  size_t count_;
  AtEnter at_enter_;
  AtExit at_exit_;
};

class StringList {
public:
  explicit StringList(std::span<std::byte> arena) {}
  ~StringList() {}

  void Add(std::string new_string) {}

  struct Node {};

  const Node* Head() const {}
  const Node* Tail() const {}
};

class OutputChannel {
public:
  OutputChannel() {}
  ~OutputChannel() {}

  void Send(std::string_view message) {}
};

int main(int argc, const char* argv[]) {
  if (argc != 2) {
    std::cerr
        << "Usage: writer <shared memory size in bytes>\n";
    return 1;
  }

  size_t size_in_bytes;
  sscanf(argv[1], "%zu", &size_in_bytes);

  SharedResource shared_resource;
  SemaphoreGuard sem_guard{
      2,
      [&shared_resource, size_in_bytes] {
        ftruncate(shared_resource.Handle(), size_in_bytes);
      },
      [&shared_resource] {
        shm_unlink(shared_resource.Name().c_str());
      }};

  SharedMemory arena(shared_resource, size_in_bytes);
  StringList string_list(arena.Access());

  OutputChannel channel;

  for (std::string line; std::getline(std::cin, line);) {
    string_list.Add(std::move(line));
    channel.Send("new_line");
  }

  return 0;
}