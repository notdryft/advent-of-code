#ifndef COMMONS
#define COMMONS

#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <sys/time.h>

#include "string.h"
#include "time.h"

// types

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

// logging

#ifdef TRACE
#define DEBUG
#define trace(...) printf(__VA_ARGS__)
#define tracef(f, ...) f(__VA_ARGS__)
#else
#define trace(...)
#define tracef(f, ...)
#endif

#ifdef DEBUG
#define INFO
#define debug(...) printf(__VA_ARGS__)
#define debugf(f, ...) f(__VA_ARGS__)
#else
#define debug(...)
#define debugf(f, ...)
#endif

#ifdef INFO
#define info(...) printf(__VA_ARGS__)
#define infof(f, ...) f(__VA_ARGS__)
#else
#define info(...)
#define infof(f, ...)
#endif

// debug

#if defined(DEBUG) || defined(TRACE)

#include <string.h>
#include <dlfcn.h>

typedef void * (*mallocf) (size_t);

#pragma GCC diagnostic ignored "-Wpedantic"
void *malloc(size_t size) {
  mallocf f = (mallocf) dlsym(RTLD_NEXT, "malloc");
  void *p = f(size);
  memset(p, 0xcd, size);

  return p;
}

#endif

// files

constexpr size_t READ_LINES_BUFFER_LENGTH = 1024*1024;

StringArray *read_lines(char filename[static 1]);

// iterators

#define macro_var(name) name ## __LINE__

#define enumerate(it, i, array, size) \
  for (size_t i = 0, _continue = 1; _continue && i < size; _continue = !_continue, i++) \
    for (it = array[i]; _continue; _continue = !_continue)

#define foreach(it, array, size) enumerate(it, macro_var(_i_), array, size)

/// Array

#define array_enumerate(it, i, array) \
  for (size_t i = 0, _continue = 1; _continue && i < array->size; _continue = !_continue, i++) \
    for (it = array_get(items, i); _continue; _continue = !_continue)

#define array_foreach(it, array) array_enumerate(it, macro_var(_i_), array)

/// StringArray

#define string_array_enumerate(it, i, array) \
  for (size_t i = 0, _continue = 1; _continue && i < array->size; _continue = !_continue, i++) \
    for (it = array->items[i]; _continue; _continue = !_continue)

#define string_array_foreach(it, array) string_array_enumerate(it, macro_var(_i_), array)

// test case

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define test_case(day, part, input, expected, ...) __extension__ ({ \
  StringArray *data = read_lines("../../inputs/" STR(AOC_YEAR) "/" STR(day) "/" STR(input)); \
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
