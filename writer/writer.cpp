#include "all.h"

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
  StringList string_list(arena.Access());

  OutputChannel channel;

  for (std::string line; std::getline(std::cin, line);) {
    string_list.Add(std::move(line));
    channel.Send("new_line");
  }

  return 0;
}