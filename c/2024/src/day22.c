#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

constexpr size_t ITERATIONS = 2000;

i64 hash(i64 secret) {
  secret ^= (secret << 6) & 0xffffff;  // * 64
  secret ^= (secret >> 5);             // / 32
  secret ^= (secret << 11) & 0xffffff; // * 2048
  return secret;
}

i64 part1(StringArray *lines) {
  i64 result = 0;

  string_array_foreach (char *line, lines) {
    i64 secret = atoll(line);

    i64 n = secret;
    for (size_t i = 0; i < ITERATIONS; i++) {
      n = hash(n);
    }

    result += n;
  }

  return result;
}

constexpr size_t BANANAS_SIZE = 1 << 20;

i64 bananas[BANANAS_SIZE] = {};
size_t seen[BANANAS_SIZE];

i64 part2(StringArray *lines) {
  i64 result = 0;

  memset(bananas, 0, sizeof(*bananas) * BANANAS_SIZE);
  for (size_t i = 0; i < BANANAS_SIZE; i++) {
    seen[i] = INT64_MAX;
  }

  string_array_enumerate (char *line, j, lines) {
    i64 n = atoll(line);

    i64 changes = 0;
    i64 old_price = n % 10;
    debug("%10lld: %lld\n", n, old_price);

    for (size_t i = 0; i < ITERATIONS; i++) {
      n = hash(n);

      i64 price = n % 10;
      i64 change = price - old_price;
      debug("%10lld: %lld (%lld)\n", n, price, change);

      // 0xfffff = 1 << 20 - 1 = 4x5 bits mask
      changes = ((changes << 5) & 0xfffff) + (change + 10);
      if (i >= 3 && seen[changes] != j) {
        seen[changes] = j;
        bananas[changes] += price;
        if (bananas[changes] > result) {
          result = bananas[changes];
        }
      }

      old_price = price;
    }
  }

  return result;
}

int main(void) {
  test_case(day22, part1, sample1, 37327623);
  test_case(day22, part1, data, 13461553007);
  test_case(day22, part2, sample2, 23);
  test_case(day22, part2, data, 1499);

  return 0;
}
