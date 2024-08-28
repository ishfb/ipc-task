#include <iostream>

#include "all.h"
#include "environment.h"
#include "log.h"
#include "string_list_view.h"

int main(int argc, const char* argv[]) {
  Environment env(argc, argv);
  const StringListView string_list(env.StringListArena());

  // InputChannel channel;

  for (std::string line; std::getline(std::cin, line);) {
    if (line == "H") {
      for (const auto& str : string_list) {
        std::cout << str << '\n';
      }
    }
    if (line == "B") {
      if (string_list.empty()) {
        LOG("StringList is empty");
      } else {
        std::cout << string_list.back() << '\n';
      }
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