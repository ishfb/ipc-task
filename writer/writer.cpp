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

  SharedResource shared_resource(2, size_in_bytes);

  SharedMemory arena(shared_resource);
  StringList string_list(arena.Access());

  OutputChannel channel;

  for (std::string line; std::getline(std::cin, line);) {
    string_list.Add(std::move(line));
    channel.Send("new_line");
  }

  return 0;
}