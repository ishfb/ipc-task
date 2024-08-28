#include "environment.h"

#include <unistd.h>

#include <csignal>
#include <cstring>
#include <iostream>

#include "log.h"
Environment::Environment(int argc, const char* argv[])
    : args_parser_(argc, argv),
      shared_resource_(args_parser_.size_in_bytes),
      shared_memory_(shared_resource_),
      ref_count_allocator_(shared_memory_.Access()),
      unlink_guard_(*ref_count_allocator_.AllocateFor<std::atomic<size_t>>(), shared_resource_.Name()),
      ipc_channel_(),
      string_list_arena_(ref_count_allocator_.FreeSpace()) {}

Environment::ArgsParser::ArgsParser(int argc, const char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: writer <shared memory size in bytes> [-v]\n";
    std::exit(1);
  }

  sscanf(argv[1], "%zu", &size_in_bytes);

  if (argc == 3 && std::string(argv[2]) == "-v") {
    SetVerbose(true);
  }

  sigset_t signalSet;
  sigfillset(&signalSet);  // Инициализируем набор, включив в него все сигналы

  // Блокируем все сигналы, кроме SIGKILL и SIGSTOP
  if (sigprocmask(SIG_BLOCK, &signalSet, nullptr) != 0) {
    std::cerr << "Failed to block all signals " << strerror(errno) << std::endl;
    std::terminate();
  } 
}
