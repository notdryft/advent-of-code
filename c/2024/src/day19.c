#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

bool find_first_match_rec(char *line, size_t line_len, size_t si, StringArray *towels) {
  if (si == line_len) {
    debug("ok\n");
    return true;
  }
  debug("%s at %zu (%.*s)\n", line, si, (int) (line_len - si + 1), line + si);

  bool result = false;
  string_array_foreach (char *towel, towels) {
    size_t towel_len = strlen(towel);
    if (strncmp(line + si, towel, towel_len) == 0) {
      debug("%s (%zu) matched %.*s (%c)\n", towel, towel_len, (int) towel_len, line + si, line[si]);
      result = result || find_first_match_rec(line, line_len, si + towel_len, towels);
    }
  }
  debug("ko\n");

  return result;
}

i64 part1(StringArray *lines) {
  i64 result = 0;

  StringArray *towels = string_split(lines->items[0], ", ");
  debugf(string_array_print, towels);

  for (size_t i = 2; i < lines->size; i++) {
    char *line = lines->items[i];
    size_t line_len = strlen(line);
    if (find_first_match_rec(line, line_len, 0, towels)) {
      result++;
    }
  }

  string_array_free(towels);

  return result;
}

typedef struct {
  char partial[100];
  i64 value;
} DPEntry;

constexpr size_t DP_KEY_STRIDE = sizeof(char) * 100;

i64 find_all_matches_rec(char *line, size_t line_len, size_t si, StringArray *towels, Map *dp) {
  if (si == line_len) {
    return 1;
  }

  DPEntry key = {};
  strncpy(key.partial, line + si, line_len - si + 1);

  DPEntry *entry = map_get(dp, &key);
  if (entry != nullptr) {
    return entry->value;
  }

  i64 result = 0;
  string_array_foreach (char *towel, towels) {
    size_t towel_len = strlen(towel);
    if (strncmp(line + si, towel, towel_len) == 0) {
      result += find_all_matches_rec(line, line_len, si + towel_len, towels, dp);
    }
  }

  key.value = result;
  map_put(dp, &key);

  return result;
}

i64 part2(StringArray *lines) {
  i64 result = 0;

  StringArray *towels = string_split(lines->items[0], ", ");
  debugf(string_array_print, towels);

  Map *dp = map_new(DPEntry, DP_KEY_STRIDE);
  for (size_t i = 2; i < lines->size; i++) {
    char *line = lines->items[i];
    size_t line_len = strlen(line);
    i64 matches = find_all_matches_rec(line, line_len, 0, towels, dp);
    debug("%s: %lld\n", line, matches);
    result += matches;
  }

  map_free(dp);
  string_array_free(towels);

  return result;
}

int main(void) {
  test_case(day19, part1, sample, 6);
  test_case(day19, part1, data, 258);
  test_case(day19, part2, sample, 16);
  test_case(day19, part2, data, 632423618484345);

  return 0;
}
