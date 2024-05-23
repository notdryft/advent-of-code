#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

i64 compute(StringArray *lines, size_t iterations) {
  Array *digits = array_new(i64);
  string_array_foreach (char *line, lines) {
    size_t line_len = strlen(line);
    for (size_t i = 0; i < line_len; i++) {
      i64 digit = line[i] - '0';
      array_push_rval(digits, digit);
    }
  }
  debugf(i64_array_print, digits);

  for (size_t it = 0; it < iterations; it++) {
    Array *new_digits = array_new(i64);
    for (size_t i = 0; i < digits->size; i++) {
      i64 count = 1;
      i64 current = i64_array_get(digits, i);
      while (i + 1 < digits->size) {
        i64 next = i64_array_get(digits, i + 1);
        if (current == next) {
          count++;
          i++;
        } else {
          break;
        }
      }
      array_push_rval(new_digits, count);
      array_push_rval(new_digits, current);
    }

    array_free(digits);
    digits = new_digits;
  }
  debugf(i64_array_print, digits);

  i64 result = digits->size;;
  array_free(digits);

  return result;
}

i64 part1(StringArray *lines) {
  return compute(lines, 40);
}

i64 part2(StringArray *lines) {
  return compute(lines, 50);
}

int main(void) {
  test_case(day10, part1, data, 329356);
  test_case(day10, part2, data, 4666278);

  return 0;
}
