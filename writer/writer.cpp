#include <cstring>
#include <iostream>

#include "environment.h"
#include "log.h"
#include "string_list.h"
#include "trie.h"

int main(int argc, const char* argv[]) {
  Environment env(argc, argv);
  StringList string_list(env.StringListArena());

  Trie trie;

  for (std::string line; std::getline(std::cin, line) && line != "Q";) {
    if (line[0] == '/') {
      trie.Gather(std::string_view(line.begin() + 1, line.end()),
                  [](std::string_view str) { std::cout << str << '\n'; });
    } else {
      string_list.Add(std::move(line));
      trie = Trie{string_list};

      env.GetIpcChannel().Send();
    }
  }

  return 0;
}