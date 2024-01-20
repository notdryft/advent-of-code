#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "string.h"

int predict_first_rec(Array *array) {
  Array *sub = array_new(int);
  for (size_t i = 1; i < array->size; i++) {
    array_push_rval(sub, int_array_get(array, i) - int_array_get(array, i - 1));
  }
  debugf(int_array_print, sub);
  if (int_array_all(sub, 0)) {
    int first = int_array_first(array);
    array_free(sub);
    debug("array first = %d, sub array predirection = 0, prediction = %d\n", first, first);
    return first;
  } else {
    int sub_prediction = predict_first_rec(sub);
    array_free(sub);
    int first = int_array_first(array);
    int prediction = first - sub_prediction;
    debug("array first = %d, sub predirection = %d, prediction = %d\n", first, sub_prediction, prediction);
    return prediction;
  }
}

int predict_last_rec(Array *array) {
  Array *sub = array_new(int);
  for (size_t i = 1; i < array->size; i++) {
    array_push_rval(sub, int_array_get(array, i) - int_array_get(array, i - 1));
  }
  debugf(int_array_print, sub);
  if (int_array_all(sub, 0)) {
    int last = int_array_last(array);
    debug("array last = %d, sub array predirection = 0, prediction = %d\n", last, last);
    array_free(sub);
    return last;
  } else {
    int sub_prediction = predict_last_rec(sub);
    int last = int_array_last(array);
    int prediction = last + sub_prediction;
    debug("array last = %d, sub array predirection = %d, prediction = %d\n", last, sub_prediction, prediction);
    array_free(sub);
    return prediction;
  }
}

int part1(StringArray *lines) {
  int result = 0;

  string_array_foreach (char *line, lines) {
    StringArray *line_split = string_split(line, " ");
    Array *array = string_atoi(line_split);
    string_array_free(line_split);
    debugf(int_array_print, array);

    int last = predict_last_rec(array);
    array_free(array);
    result += last;
  }

  return result;
}

int part2(StringArray *lines) {
  int result = 0;

  string_array_foreach (char *line, lines) {
    StringArray *line_split = string_split(line, " ");
    Array *array = string_atoi(line_split);
    string_array_free(line_split);
    debugf(int_array_print, array);

    int first = predict_first_rec(array);
    array_free(array);
    result += first;
  }

  return result;
}

int main(void) {
  test_case(day9, part1, sample, 114);
  test_case(day9, part1, data, 1842168671);
  test_case(day9, part2, sample, 2);
  test_case(day9, part2, data, 903);

  return 0;
}
