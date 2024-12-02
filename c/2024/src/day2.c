#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

ssize_t first_unsafe_level(Array *array) {
  i64 levels = 0;

  for (size_t i = 1; i < array->size; i++) {
    i64 previous = i64_array_get(array, i - 1);
    i64 current = i64_array_get(array, i);

    i64 level = current - previous;
    debug("%lld - %lld = %lld\n", current, previous, level);
    if (level == 0 || level > 3 || level < -3) {
      return i;
    }

    if ((level > 0 && levels < 0) || (level < 0 && levels > 0)) {
      return i;
    }

    levels += level;
  }

  return -1;
}

i64 part1(StringArray *lines) {
  i64 result = 0;

  string_array_foreach (char *line, lines) {
    StringArray* split = string_split(line, " ");
    debugf(string_array_print, split);

    Array *steps = string_atoll(split);
    ssize_t unsafe = first_unsafe_level(steps);
    if (unsafe == -1) {
      result++;
    }

    array_free(steps);
    string_array_free(split);
  }

  return result;
}

i64 part2(StringArray *lines) {
  i64 result = 0;

  string_array_foreach (char *line, lines) {
    StringArray* split = string_split(line, " ");
    debugf(string_array_print, split);

    Array* steps = string_atoll(split);
    debugf(i64_array_print, steps);

    ssize_t unsafe = first_unsafe_level(steps);
    if (unsafe == -1) {
      result++;
    } else {
      for (size_t i = 0; i < steps->size; i++) {
        Array *dup = array_dup(steps);
        array_remove(dup, i);
        unsafe = first_unsafe_level(dup);
        if (unsafe == -1) {
          result++;
          break;
        }
        
        array_free(dup);
      }
    }

    array_free(steps);
    string_array_free(split);
  }

  return result;
}

int main(void) {
  test_case(day2, part1, sample, 2);
  test_case(day2, part1, data, 631);
  test_case(day2, part2, sample, 4);
  test_case(day2, part2, data, 665);

  return 0;
}
