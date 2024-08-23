
#include <unistd.h>

#include <iostream>
#include <span>
#include <string>

class SharedResource;
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
