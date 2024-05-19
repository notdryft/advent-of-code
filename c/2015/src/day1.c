#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

i64 part1(StringArray *lines) {
  i64 result = 0;

  string_array_foreach (char *line, lines) {
    size_t line_len = strlen(line);
    debug("%s: %zu\n", line, line_len);
    for (size_t i = 0; i < line_len; i++) {
      if (line[i] == '(') {
        result++;
      } else if (line[i] == ')') {
        result--;
      }
    }
  }

  return result;
}

i64 part2(StringArray *lines) {
  i64 floor = 0;
  i64 result = 0;

  for (size_t j = 0; j < lines->size; j++) {
    char *line = string_array_get(lines, j);
    size_t line_len = strlen(line);
    debug("%s: %zu\n", line, line_len);

    for (size_t i = 0; i < line_len; i++) {
      if (line[i] == '(') {
        floor++;
      } else if (line[i] == ')') {
        floor--;
      }
      if (floor == -1) {
        result = i + 1;
        goto defer;
      }
    }
  }

  defer:
  return result;
}

int main(void) {
  test_case(day1, part1, data, 280);
  test_case(day1, part2, data, 1797);

  return 0;
}
