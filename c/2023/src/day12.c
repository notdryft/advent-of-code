#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "map.h"
#include "string.h"

bool arrangements_cmp(Array *a, Array *b) {
  if (a->size != b->size) {
    return false;
  }
  for (size_t i = 0; i < a->size; i++) {
    if (int_array_get(a, i) != int_array_get(b, i)) {
      return false;
    }
  }
  return true;
}

bool is_valid(char *springs, Array *damaged, size_t len) {
  Array *d = array_new(int);
  int j = 0;
  bool s = false;
  for (size_t i = 0; i < len; i++) {
    if (springs[i] == '#' && !s) {
      j = i;
      s = true;
    }
    if (springs[i] == '.' && s) {
      array_push_rval(d, i - j);
      s = false;
    }
  }
  if (s) {
    array_push_rval(d, len - j);
  }
  //int_array_print(d);

  bool result = arrangements_cmp(damaged, d);
  array_free(d);

  return result;
}

int find_arrangements_rec(char *springs, Array *damaged, size_t len, size_t i) {
  //printf("find_arrangements_rec(\"%s\", %zu)\n", springs, i);
  if (i == len) {
    return is_valid(springs, damaged, len) ? 1 : 0;
  } else if (springs[i] == '?') {
    char *copy1 = strdup(springs);
    char *copy2 = strdup(springs);
    copy1[i] = '.';
    copy2[i] = '#';

    int a = find_arrangements_rec(copy1, damaged, len, i + 1);
    int b = find_arrangements_rec(copy2, damaged, len, i + 1);

    free(copy1);
    free(copy2);

    return a + b;
  } else {
    return find_arrangements_rec(springs, damaged, len, i + 1);
  }
}

typedef struct {
  size_t si;
  size_t di;
  int dlen;
  unsigned long long value;
} DPEntry;

constexpr size_t DP_KEY_STRIDE = sizeof(size_t) * 2 + sizeof(int);

#define dp_get(dp, ...) map_get(dp, &(DPEntry) { __VA_ARGS__ })
#define dp_put(dp, ...) map_put(dp, &(DPEntry) { __VA_ARGS__ })

unsigned long long find_arrangements_dp(char *springs, Array *damaged, Map *dp, size_t si, size_t di, int dlen) {
  DPEntry *entry = dp_get(dp, .si = si, .di = di, .dlen = dlen);
  if (entry != nullptr) {
    return entry->value;
  }
  //debug("cache miss: (%zu, %zu, %d)\n", si, di, dlen);
  if (si == strlen(springs)) {
    //debug("case: (%zu, %zu, %d), %zu, %d\n", si, di, dlen, damaged->size, int_array_get(damaged, di));
    if (di == damaged->size && dlen == 0) {
      return 1;
    } else if (di == damaged->size - 1 && int_array_get(damaged, di) == dlen) {
      return 1;
    } else {
      return 0;
    }
  }
  char cs[] = ".#";
  unsigned long long arrangements = 0;
  for (size_t i = 0; i < strlen(cs); i++) {
    char c = cs[i];
    if (springs[si] == c || springs[si] == '?') {
      if (c == '.' && dlen == 0) {
        arrangements += find_arrangements_dp(springs, damaged, dp, si + 1, di, 0);
      } else if (c == '.' && dlen > 0 && di < damaged->size && int_array_get(damaged, di) == dlen) {
        arrangements += find_arrangements_dp(springs, damaged, dp, si + 1, di + 1, 0);
      } else if (c == '#') {
        arrangements += find_arrangements_dp(springs, damaged, dp, si + 1, di, dlen + 1);
      }
    }
  }
  //debug("cache push: (%zu, %zu, %d) -> %llu\n", si, di, dlen, arrangements);
  dp_put(dp, .si = si, .di = di, .dlen = dlen, .value = arrangements);
  return arrangements;
}

int part1(StringArray *lines) {
  int result = 0;

  for (size_t i = 0; i < lines->size; i++) {
    char *record = string_array_get(lines, i);
    debug("%s\n", record);

    StringArray* split = string_split(record, " ");
    char *springs = string_array_get(split, 0);
    StringArray *damaged_stra = string_split(string_array_get(split, 1), ",");
    Array *damaged = string_atoi(damaged_stra);

    int arrangements = find_arrangements_rec(springs, damaged, strlen(springs), 0);
    debug("result += %d\n", arrangements);
    result += arrangements;

    array_free(damaged);
    string_array_free(damaged_stra);
    string_array_free(split);
  }

  return result;
}

unsigned long long part2(StringArray *lines) {
  unsigned long long result = 0;

  for (size_t i = 0; i < lines->size; i++) {
    char *record = string_array_get(lines, i);
    debug("%s\n", record);

    StringArray* split = string_split(record, " ");
    char *springs = string_array_get(split, 0);
    char *springs_repeated = repeat_with_delimiter(springs, 5, "?");
    debug("repeated = %s\n", springs_repeated);

    char *damaged_str = string_array_get(split, 1);
    StringArray *damaged_stra = string_split(damaged_str, ",");
    StringArray *damaged_repeated_stra = string_array_repeat(damaged_stra, 5);
    Array *damaged = string_atoi(damaged_repeated_stra);
    debugf(int_array_print, damaged);

    Map* dp = map_new(DPEntry, DP_KEY_STRIDE);
    unsigned long long arrangements = find_arrangements_dp(springs_repeated, damaged, dp, 0, 0, 0);
    debug("result += %llu\n", arrangements);
    result += arrangements;

    array_free(damaged);
    free(springs_repeated);
    map_free(dp);
    string_array_free(damaged_repeated_stra);
    string_array_free(damaged_stra);
    string_array_free(split);
  }

  return result;
}

int main(void) {
  test_case(day12, part1, sample, 21);
  test_case(day12, part1, data, 7694);
  test_case(day12, part2, sample, 525152);
  test_case(day12, part2, data, 5071883216318);

  return 0;
}
