#include <sys/mman.h>
#include <unistd.h>

#include <iostream>
#include <span>
#include <string>

class SharedResource;

class SharedMemory {
public:
  explicit SharedMemory(
      const SharedResource& shared_resource, size_t bytes) {
  }
  ~SharedMemory() {}

  std::span<std::byte> Access() {}
};

template <typename AtEnter, typename AtExit>
class SemaphoreGuard {
public:
  explicit SemaphoreGuard(size_t count, AtEnter at_enter,
                          AtExit at_exit)
      : count_(count),
        at_enter_(at_enter),
        at_exit_(at_exit) {}

  ~SemaphoreGuard() {}

private:
  size_t count_;
  AtEnter at_enter_;
  AtExit at_exit_;
};

class StringList {
public:
  explicit StringList(std::span<std::byte> arena) {}
  ~StringList() {}

  void Add(std::string new_string) {}

  using iterator = int*;
  using const_iterator = const int*;

  iterator begin() {}
  iterator end() {}
  const_iterator begin() const {} 
  const_iterator end() const {}

  const std::string& back() const {}  
};

class OutputChannel {
public:
  OutputChannel() {}
  ~OutputChannel() {}

  void Send(std::string_view message) {}
};

class InputChannel {
public:
  InputChannel() {}
  ~InputChannel() {}

  bool TryReceive(std::string& message) {}
};

