#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "map.h"
#include "string.h"

#define BUFFER_LENGTH 1024

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
      array_push(d, i - j);
      s = false;
    }
  }
  if (s) {
    array_push(d, len - j);
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
} DPKey;

typedef struct {
  size_t si;
  size_t di;
  int dlen;
  unsigned long long value;
} DPEntry;

#define dp_get(dp, ...) map_get(dp, &(DPEntry) { __VA_ARGS__ })
#define dp_put(dp, ...) map_put(dp, &(DPEntry) { __VA_ARGS__ })

unsigned long long find_arrangements_dp(char *springs, Array *damaged, Map *dp, size_t si, size_t di, int dlen) {
  DPEntry *entry = dp_get(dp, .si = si, .di = di, .dlen = dlen);
  if (entry != nullptr) {
    return entry->value;
  }
  //printf("cache miss: (%zu, %zu, %zu)\n", si, di, dlen);
  if (si == strlen(springs)) {
    //printf("case: (%zu, %zu, %zu), %zu, %d\n", si, di, dlen, damaged->size, int_array_get(damaged, di));
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
  //printf("cache push: (%zu, %zu, %zu) -> %llu\n", si, di, dlen, arrangements);
  dp_put(dp, .si = si, .di = di, .dlen = dlen, .value = arrangements);
  return arrangements;
}

int part1(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == nullptr) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  StringArray *records = string_array_new();

  char buffer[BUFFER_LENGTH] = {};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';
    string_array_push(records, buffer);
  }
  fclose(fp);

  int arrangements = 0;

  for (size_t i = 0; i < records->size; i++) {
    char *record = string_array_get(records, i);
    printf("%s\n", record);

    StringArray* split = string_split(record, " ");
    char *springs = string_array_get(split, 0);
    StringArray *damaged_stra = string_split(string_array_get(split, 1), ",");
    Array *damaged = string_atoi(damaged_stra);

    int a = find_arrangements_rec(springs, damaged, strlen(springs), 0);
    printf("arrangements += %d\n", a);
    arrangements += a;

    array_free(damaged);
    string_array_free(damaged_stra);
    string_array_free(split);
  }
  string_array_free(records);


  printf("arrangements = %d\n", arrangements);

  return arrangements;
}

unsigned long long part2(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == nullptr) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  StringArray *records = string_array_new();

  char buffer[BUFFER_LENGTH] = {};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';
    string_array_push(records, buffer);
  }
  fclose(fp);

  unsigned long long arrangements = 0;

  for (size_t i = 0; i < records->size; i++) {
    char *record = string_array_get(records, i);
    printf("%s\n", record);

    StringArray* split = string_split(record, " ");
    char *springs = string_array_get(split, 0);
    char *springs_repeated = repeat_with_delimiter(springs, 5, "?");
    printf("repeated = %s\n", springs_repeated);

    char *damaged_str = string_array_get(split, 1);
    StringArray *damaged_stra = string_split(damaged_str, ",");
    StringArray *damaged_repeated_stra = string_array_repeat(damaged_stra, 5);
    Array *damaged = string_atoi(damaged_repeated_stra);
    int_array_print(damaged);

    Map* dp = map_new(DPKey, DPEntry);
    unsigned long long a = find_arrangements_dp(springs_repeated, damaged, dp, 0, 0, 0);
    printf("arrangements += %llu\n", a);
    arrangements += a;

    array_free(damaged);
    free(springs_repeated);
    map_free(dp);
    string_array_free(damaged_repeated_stra);
    string_array_free(damaged_stra);
    string_array_free(split);
  }
  string_array_free(records);

  printf("arrangements = %llu\n", arrangements);

  return arrangements;
}

int main(void) {
  assert(part1("../../inputs/2023/day12/sample") == 21);
  assert(part1("../../inputs/2023/day12/data") == 7694);
  assert(part2("../../inputs/2023/day12/sample") == 525152);
  assert(part2("../../inputs/2023/day12/data") == 5071883216318);

  return 0;
}
