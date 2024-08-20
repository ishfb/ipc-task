#include "all.h"
#include "shared_resource.h"

#include <iostream>

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
  const StringList string_list(arena.Access());

  InputChannel channel;

  for (std::string line; ;) {
    if (std::getline(std::cin, line) && line == "H") {
      for (const auto& str : string_list) {
        std::cout << str << '\n';
      }
    }
    if (channel.TryReceive(line) && line == "new_line") {
      std::cout << string_list.back() << '\n';
    } 
  }

  return 0;
}