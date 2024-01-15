#ifndef COMMONS
#define COMMONS

#include <assert.h>
#include <inttypes.h>
#include <stdint.h>

#include "string.h"
#include "time.h"

typedef long long ll;
typedef long double ld;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
__extension__ typedef __int128_t i128;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
__extension__ typedef __uint128_t u128;

__extension__ typedef __float128 f128;

#ifdef DEBUG
#define debug(...) printf(__VA_ARGS__)
#define debugf(f, ...) f(__VA_ARGS__)
#else
#define debug(...)
#define debugf(f, ...)
#endif

constexpr size_t READ_LINES_BUFFER_LENGTH = 1024*1024;

StringArray *read_lines(char filename[static 1]);

#define test_case(day, part, input, expected, ...) __extension__ ({ \
  StringArray *data = read_lines("../../inputs/2023/" #day "/" #input); \
  struct timeval start = now(); \
  i64 result = (i64) part(data __VA_OPT__(,) __VA_ARGS__); \
  struct timeval end = now(); \
  string_array_free(data); \
  printf(#day "\t" #part "\t" #input "\t%" PRId64, (i64) result); \
  Time elapsed = interval(start, end); \
  if (elapsed.seconds > 0) { \
    printf("\t%" PRIu64 "s %" PRIu64 "ms\n", elapsed.seconds, elapsed.milliseconds); \
  } else if (elapsed.milliseconds > 0) { \
    printf("\t%" PRIu64 "ms %" PRIu64 "µs\n", elapsed.milliseconds, elapsed.microseconds); \
  } else { \
    printf("\t%" PRIu64 "µs\n", elapsed.microseconds); \
  } \
  assert(result == expected); \
})

#endif
