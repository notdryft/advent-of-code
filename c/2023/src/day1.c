#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "string.h"

int part1(StringArray *lines) {
  int result = 0;

  string_array_foreach (char *line, lines) {
    size_t line_len = strlen(line);

    Array *digits = array_new(int);
    for (size_t i = 0; i < line_len; i++) {
      int value = line[i] - '0';
      if (value > 0 && value < 10) {
        array_push_rval(digits, value);
      }
    }
    debugf(int_array_print, digits);
    result += int_array_first(digits) * 10 + int_array_last(digits);

    array_free(digits);
  }

  return result;
}

int part2(StringArray *lines) {
  int result = 0;

  string_array_foreach (char *line, lines) {
    size_t line_len = strlen(line);
    debug("%s: %zu\n", line, line_len);

    Array *digits = array_new(int);
    for (size_t i = 0; i < line_len; i++) {
      char c = line[i];
      if (c >= '1' && c <= '9') {
        array_push_rval(digits, c - '0');
      } else {
        if (strncmp(line + i, "one", 3) == 0) array_push_rval(digits, 1);
        else if (strncmp(line + i, "two", 3) == 0) array_push_rval(digits, 2);
        else if (strncmp(line + i, "three", 5) == 0) array_push_rval(digits, 3);
        else if (strncmp(line + i, "four", 4) == 0) array_push_rval(digits, 4);
        else if (strncmp(line + i, "five", 4) == 0) array_push_rval(digits, 5);
        else if (strncmp(line + i, "six", 3) == 0) array_push_rval(digits, 6);
        else if (strncmp(line + i, "seven", 5) == 0) array_push_rval(digits, 7);
        else if (strncmp(line + i, "eight", 5) == 0) array_push_rval(digits, 8);
        else if (strncmp(line + i, "nine", 4) == 0) array_push_rval(digits, 9);
      }
    }
    debugf(int_array_print, digits);
    result += int_array_first(digits) * 10 + int_array_last(digits);
    array_free(digits);
  }

  return result;
}

int main(void) {
  test_case(day1, part1, sample1, 142);
  test_case(day1, part1, data, 55488);
  test_case(day1, part2, sample2, 281);
  test_case(day1, part2, data, 55614);

  return 0;
}
