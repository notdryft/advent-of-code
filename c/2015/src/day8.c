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

    size_t in_memory = 0;
    for (size_t i = 1; i < line_len - 1; i++) {
      trace("%c\n", line[i]);
      if (line[i] == '\\') {
        if (line[i + 1] == '\\' || line[i + 1] == '"') {
          in_memory++;
          i++;
        } else if (line[i + 1] == 'x') {
          in_memory++;
          i += 3;
        } else {
          printf("unknown escaped char at position %zu: %c", i + 1, line[i + 1]);
        }
      } else {
        in_memory++;
      }
    }

    debug("%zu - %zu\n", line_len, in_memory);
    result += line_len - in_memory;
  }

  return result;
}

i64 part2(StringArray *lines) {
  i64 result = 0;

  string_array_foreach (char *line, lines) {
    size_t line_len = strlen(line);
    debug("%s: %zu\n", line, line_len);

    size_t representation = 6;
    for (size_t i = 1; i < line_len - 1; i++) {
      trace("%c\n", line[i]);
      if (line[i] == '\\') {
        if (line[i + 1] == '\\' || line[i + 1] == '"') {
          representation += 4;
          i++;
        } else if (line[i + 1] == 'x') {
          representation += 5;
          i += 3;
        } else {
          printf("unknown escaped char at position %zu: %c", i + 1, line[i + 1]);
        }
      } else {
        representation++;
      }
    }

    debug("%zu - %zu\n", representation, line_len);
    result += representation - line_len;
  }

  return result;
}

int main(void) {
  //test_case(day8, part1, sample, 12);
  //test_case(day8, part1, data, 0);
  //test_case(day8, part2, sample, 19);
  test_case(day8, part2, data, 0);

  return 0;
}
