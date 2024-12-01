#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

int i64_cmp(const void *a, const void *b) {
  return (*(i64 *) a) - (*(i64 *) b);
}

i64 part1(StringArray *lines) {
  i64 result = 0;

  Array *left = array_new(i64);
  Array *right = array_new(i64);

  string_array_foreach (char *line, lines) {
    i64 a, b;
    if (sscanf(line, "%lld   %lld", &a, &b) == 2) {
      debug("%lld %lld\n", a, b);

      array_push_rval(left, a);
      array_push_rval(right, b);
    }
  }

  array_sort(left, i64_cmp);
  array_sort(right, i64_cmp);

  debugf(i64_array_print, left);
  debugf(i64_array_print, right);

  for (size_t i = 0; i < left->size; i++) {
    i64 a = i64_array_get(left, i);
    i64 b = i64_array_get(right, i);

    result += a - b < 0 ? b - a : a - b;
  }

  array_free(left);
  array_free(right);

  return result;
}

i64 part2(StringArray *lines) {
  i64 result = 0;

  Array *left = array_new(i64);
  Array *right = array_new(i64);

  string_array_foreach (char *line, lines) {
    i64 a, b;
    if (sscanf(line, "%lld   %lld", &a, &b) == 2) {
      debug("%lld %lld\n", a, b);

      array_push_rval(left, a);
      array_push_rval(right, b);
    }
  }

  debugf(i64_array_print, left);
  debugf(i64_array_print, right);

  for (size_t i = 0; i < left->size; i++) {
    i64 a = i64_array_get(left, i);

    i64 similarity = 0;
    for (size_t j = 0; j < right->size; j++) {
      i64 b = i64_array_get(right, j);
      if (a == b) {
        similarity++;
      }
    }

    result += a * similarity;
  }

  array_free(left);
  array_free(right);

  return result;
}

int main(void) {
  test_case(day1, part1, sample, 11);
  test_case(day1, part1, data, 3508942);
  test_case(day1, part2, sample, 31);
  test_case(day1, part2, data, 26593248);

  return 0;
}
