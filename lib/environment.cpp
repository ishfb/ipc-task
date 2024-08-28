#include "environment.h"

#include <iostream>

Environment::Environment(int argc, const char* argv[])
    : args_parser_(argc, argv),
      shared_resource_(args_parser_.size_in_bytes),
      shared_memory_(shared_resource_),
      ref_count_allocator_(shared_memory_.Access()),
      unlink_guard_(*ref_count_allocator_.AllocateFor<size_t>(nullptr), shared_resource_.Name()),
      string_list_arena_(ref_count_allocator_.FreeSpace()) {}

Environment::ArgsParser::ArgsParser(int argc, const char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: writer <shared memory size in bytes> [--verbose]\n";
    std::exit(1);
  }

  sscanf(argv[1], "%zu", &size_in_bytes);

  if (argc == 3 && std::string(argv[2]) == "--verbose") {
    verbose = true;
  }
}
