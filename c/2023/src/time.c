#include <sys/time.h>

#include "time.h"

struct timeval now() {
  struct timeval t;
  gettimeofday(&t, nullptr);
  return t;
}

Time interval(struct timeval start, struct timeval end) {
  uint64_t seconds = end.tv_sec - start.tv_sec;
  uint64_t microseconds = end.tv_usec - start.tv_usec;
  return (Time) {
    .seconds = seconds,
    .milliseconds = (microseconds / 1000) % 1000,
    .microseconds = microseconds % 1000
  };
}
