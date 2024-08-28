#include <atomic>
#include <iostream>
#include <thread>

#include "environment.h"
#include "log.h"
#include "string_list_view.h"

int main(int argc, const char* argv[]) {
  Environment env(argc, argv);
  const StringListView string_list(env.StringListArena());

  std::atomic_flag should_stop = ATOMIC_FLAG_INIT;
  std::thread ipc_thread([&] {
    while (!should_stop.test()) {
      if (env.GetIpcChannel().TryReceive()) {
        std::cout << string_list.back() << '\n';
      }
    }
  });

 for (std::string line; std::getline(std::cin, line) && line != "Q";) {
     if (line == "H") {
      for (const auto& str : string_list) {
        std::cout << str << '\n';
      }
    }
  }

  should_stop.test_and_set();
  ipc_thread.join();

  return 0;
}