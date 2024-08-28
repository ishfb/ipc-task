#include <cstring>
#include <iostream>

#include "all.h"
#include "log.h"
#include "string_list.h"
#include "environment.h"

int main(int argc, const char* argv[]) {
  Environment env(argc, argv);
  StringList string_list(env.StringListArena());

  // OutputChannel channel;

  for (std::string line; std::getline(std::cin, line) && line != "exit";) {
    string_list.Add(std::move(line));
    // channel.Send("new_line");
    LOG("Got: " << line);
  }

  return 0;
}