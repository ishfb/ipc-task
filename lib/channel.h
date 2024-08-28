#pragma once

#include <semaphore.h>

#include <atomic>

class IpcChannel {
public:
  IpcChannel();
  ~IpcChannel();

  void Send();
  bool TryReceive();

private:
  sem_t* semaphore_;
};
