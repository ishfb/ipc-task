#include "channel.h"

IpcChannel::IpcChannel(std::atomic<size_t>& counter) : counter_(counter) {}

void IpcChannel::Send() { ++counter_; }

bool IpcChannel::TryReceive() {
  auto counter_value = counter_.load();
  if (counter_value > 0) {
    return counter_.compare_exchange_weak(counter_value, counter_value - 1);
  }
  return false;
}
