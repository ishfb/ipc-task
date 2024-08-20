#pragma once

#include <string>

class SharedResource {
public:
  SharedResource(std::string name = "");
  ~SharedResource();

  using ShmId = int;
  ShmId Handle() const { return handle_; }
  const std::string& Name() const { return name_; }

  void Cleanup();

private:
  std::string name_;
  ShmId handle_;
};

