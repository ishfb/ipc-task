
#include <unistd.h>

#include <iostream>
#include <span>
#include <string>

class SharedResource;

class SharedMemory {
public:
  explicit SharedMemory(
      const SharedResource& shared_resource) {
  }
  ~SharedMemory() {}

  std::span<std::byte> Access() {}
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

