#pragma once

#include <string>

class SharedResource {
public:
  SharedResource(size_t size_in_bytes, const std::string& name = "");
  ~SharedResource();

  using ShmId = int;
  ShmId Handle() const { return handle_; }
  const std::string& Name() const { return name_; }
  size_t SizeInBytes() const { return size_in_bytes_; }

private:
  std::string name_;
  ShmId handle_;
  size_t size_in_bytes_;
};

