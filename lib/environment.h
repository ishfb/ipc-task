#pragma once

#include <span>

#include "ring_buffer_allocator.h"
#include "shared_memory.h"
#include "shared_resource.h"
#include "unlink_guard.h"

class Environment {
public:
  Environment(int argc, const char* argv[]);

  std::span<char> StringListArena() const { return string_list_arena_; }

private:
  struct ArgsParser {
    size_t size_in_bytes;
    bool verbose = false;

    ArgsParser(int argc, const char* argv[]);
  };

  ArgsParser args_parser_;
  SharedResource shared_resource_;
  SharedMemory shared_memory_;
  LinearAllocator ref_count_allocator_;
  UnlinkGuard unlink_guard_;
  std::span<char> string_list_arena_;
};