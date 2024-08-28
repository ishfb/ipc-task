#include "channel.h"

#include <fcntl.h>    /* For O_* constants */
#include <sys/stat.h> /* For mode constants */

#include <iostream>
#include <string>

#include "log.h"

const std::string kSemaphoreDefaultName = "/ipc_task_semaphore";

IpcChannel::IpcChannel() : semaphore_(sem_open(kSemaphoreDefaultName.c_str(), O_CREAT, S_IRWXU, 0)) {
  if (semaphore_ == SEM_FAILED) {
    std::cerr << "Failed to create semaphore " << kSemaphoreDefaultName << ", errno = " << errno << std::endl;
    std::terminate();
  }
}

IpcChannel::~IpcChannel() {
  auto ret = sem_close(semaphore_);
  LOG("sem_close ret = " << ret << ", errno = " << errno);
}

void IpcChannel::Send() { sem_post(semaphore_); }

bool IpcChannel::TryReceive() {
  struct timespec timeout;
  clock_gettime(CLOCK_REALTIME, &timeout);
  timeout.tv_sec += 1;  // Set timeout to 1 second

  return sem_timedwait(semaphore_, &timeout) == 0;
}
