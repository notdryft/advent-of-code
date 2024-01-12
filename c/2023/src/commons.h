#ifndef COMMONS
#define COMMONS

#include <inttypes.h>
#include <stdint.h>

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

#endif
