#include <iostream>

#include "all.h"
#include "log.h"
#include "shared_memory.h"
#include "shared_resource.h"
#include "string_list.h"

int main(int argc, const char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: writer <shared memory size in bytes>\n";
    return 1;
  }

  size_t size_in_bytes;
  sscanf(argv[1], "%zu", &size_in_bytes);

  SharedResource shared_resource(2, size_in_bytes);

  SharedMemory arena(shared_resource);
  const StringListView string_list(arena.Access());

  // InputChannel channel;

  for (std::string line; std::getline(std::cin, line);) {
    if (line == "H") {
      for (const auto& str : string_list) {
        std::cout << str << '\n';
      }
    }
    if (line == "B") {
      std::cout << string_list.back() << '\n';
    }
    if (line == "exit") {
      break;
    }
    // if (channel.TryReceive(line) && line == "new_line") {
    //   std::cout << string_list.back() << '\n';
    // }
  }

  return 0;
}