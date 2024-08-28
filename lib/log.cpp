#include "log.h"

#include <iostream>

std::ostream* g_log_stream = nullptr;

void SetVerbose(bool verbose) {
  g_log_stream = verbose ? &std::cerr : nullptr;
}