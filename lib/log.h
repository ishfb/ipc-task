#pragma once

#include <iostream>

extern std::ostream* g_log_stream;

void SetVerbose(bool verbose);

#define LOG(...)                                        \
  do {                                                  \
    if (g_log_stream) {                                 \
      *g_log_stream << "[log] " << __VA_ARGS__ << '\n'; \
    }                                                   \
  } while (false)