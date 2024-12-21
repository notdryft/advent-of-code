#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

char numpad[] = {
  ' ', '0', 'A',
  '1', '2', '3',
  '4', '5', '6',
  '7', '8', '9',
}; // 11 keys

char keypad[] = {
  '<', 'v', '>',
  ' ', '^', 'A'
}; // 5 keys

constexpr size_t KEYS = 16;
constexpr size_t DEPTH = 26;
constexpr size_t DP_SIZE = DEPTH * KEYS * KEYS;

i64 dp[DP_SIZE];

size_t find_key(char *pad, char key) {
  size_t i = 0;
  while (true) {
    if (pad[i] == key) {
      return i;
    }
    i++;
  }
}

size_t fill_path(char *path, size_t s, size_t e, size_t y) {
  size_t sx = s % 3, sy = s / 3;
  size_t ex = e % 3, ey = e / 3;

  size_t ax = sx > ex ? sx - ex : ex - sx;
  size_t ay = sy > ey ? sy - ey : ey - sy;

  size_t path_len = 0;
  for (size_t i = 0; i < ax; i++) {
    path[path_len++] = sx > ex ? '<' : '>';
  }
  for (size_t i = 0; i < ay; i++) {
    path[path_len++] = sy > ey ? 'v' : '^';
  }

  if ((ex > sx && (sx != 0 || ey != y)) || (ex == 0 && sy == y)) {
    size_t h = path_len / 2;
    for (size_t i = 0; i < h; i++) {
      char tmp = path[i];
      path[i] = path[path_len - 1 - i];
      path[path_len - 1 - i] = tmp;
    }
  }
  path[path_len++] = 'A';

  debug("%zu -> %zu = %s (%zu)\n", s, e, path, path_len);

  return path_len;
}

i64 compute_rec(char *path, size_t path_len, size_t depth, size_t pads) {
  if (depth == pads) {
    return path_len;
  } else {
    char *pad = depth == 0 ? numpad : keypad;
    size_t s = find_key(pad, 'A');

    i64 result = 0;
    for (size_t i = 0; i < path_len; i++) {
      char new_path[8] = {};
      size_t e = find_key(pad, path[i]);
      size_t dp_key = depth * KEYS * KEYS + e * KEYS + s;
      if (dp[dp_key] != 0) {
        result += dp[dp_key];
      } else {
        debug("%zu: %c (%zu) -> %c (%zu)\n", depth, i == 0 ? 'A' : path[i - 1], s, path[i], e);

        size_t new_path_len = fill_path(new_path, s, e, depth != 0);
        i64 tmp = compute_rec(new_path, new_path_len, depth + 1, pads);
        dp[dp_key] = tmp;
        result += tmp;
      }
      s = e;
    }
    return result;
  }
}

i64 part(StringArray *lines, size_t pads) {
  i64 result = 0;

  string_array_foreach (char *line, lines) {
    size_t line_len = strlen(line);
 
    memset(dp, 0, sizeof(i64) * DP_SIZE);
    i64 min_len = compute_rec(line, line_len, 0, pads + 1);

    char *number_str = strdup(line);
    number_str[3] = '\0';
    i64 number = atoll(number_str);
    free(number_str);

    debug("%s: %lld * %lld = %lld \n", line, min_len, number, number * min_len);

    result += min_len * number;
  }

  return result;
}

int main(void) {
  // part 1
  test_case(day21, part, sample, 126384, 2);
  test_case(day21, part, data, 125742, 2);
  // part 2
  test_case(day21, part, sample, 154115708116294, 25);
  test_case(day21, part, data, 157055032722640, 25);

  return 0;
}
