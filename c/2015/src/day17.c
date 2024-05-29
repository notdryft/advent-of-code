#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

i64 rec(Array *containers, size_t index, i64 eggnog) {
  if (eggnog == 0) {
    return 1;
  } else if (eggnog < 0 || index >= containers->size) {
    return 0;
  } else {
    i64 container = i64_array_get(containers, index);
    i64 nexnog = eggnog - container;
    return (nexnog >= 0 ? rec(containers, index + 1, nexnog) : 0)
                        + rec(containers, index + 1, eggnog);
  }
}

i64 part1(StringArray *lines, i64 eggnog) {
  Array *containers = array_new(i64);
  string_array_foreach (char *line, lines) {
    //size_t line_len = strlen(line);
    //debug("%s: %zu\n", line, line_len);
    i64 container = atoll(line);
    array_push_rval(containers, container);
  }
  debugf(i64_array_print, containers);

  return rec(containers, 0, eggnog);
}

void rec2(Array *containers, size_t index, i64 eggnog, Array *current, Array *combinations) {
  if (eggnog == 0) {
    Array *copy = array_dup(current);
    array_push_pointer(combinations, copy);
  } else if (eggnog > 0 && index < containers->size) {
    i64 container = i64_array_get(containers, index);
    i64 nexnog = eggnog - container;
    if (nexnog >= 0) {
      array_push_rval(current, container);
      rec2(containers, index + 1, nexnog, current, combinations);
      array_remove_last(current);
    }
    rec2(containers, index + 1, eggnog, current, combinations);
  }
}

i64 part2(StringArray *lines, i64 eggnog) {
  Array *containers = array_new(i64);
  string_array_foreach (char *line, lines) {
    //size_t line_len = strlen(line);
    //debug("%s: %zu\n", line, line_len);
    i64 container = atoll(line);
    array_push_rval(containers, container);
  }
  debugf(i64_array_print, containers);

  Array *current = array_new(i64);
  Array *combinations = array_new(Array *);
  rec2(containers, 0, eggnog, current, combinations);
#ifdef DEBUG
  for (size_t i = 0; i < combinations->size; i++) {
    Array *array = array_get_pointer(combinations, i);
    i64_array_print(array);
  }
#endif

  size_t min = INT64_MAX;
  for (size_t i = 0; i < combinations->size; i++) {
    Array *array = array_get_pointer(combinations, i);
    if (array->size < min) {
      min = array->size;
    }
  }

  i64 result = 0;
  for (size_t i = 0; i < combinations->size; i++) {
    Array *array = array_get_pointer(combinations, i);
    if (array->size == min) {
      result++;
    }
    array_free(array);
  }
  array_free(combinations);
  array_free(current);

  return result;
}

int main(void) {
  test_case(day17, part1, sample, 4, 25);
  test_case(day17, part1, data, 1304, 150);
  test_case(day17, part2, sample, 3, 25);
  test_case(day17, part2, data, 18, 150);

  return 0;
}
