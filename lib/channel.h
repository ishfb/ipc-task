#pragma once

#include <atomic>

class IpcChannel {
public:
  IpcChannel(std::atomic<size_t>& counter);

  void Send();
  bool TryReceive();

private:
  std::atomic<size_t>& counter_;
};
