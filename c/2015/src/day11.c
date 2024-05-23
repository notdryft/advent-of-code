#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"


bool rule1_applies(char *str, size_t str_len) {
  for (size_t i = 0; i < str_len - 2; i++) {
    if (str[i + 2] == str[i + 1] + 1 && str[i + 1] == str[i] + 1) {
      return true;
    }
  }
  return false;
}

bool rule2_applies(char *str, size_t str_len) {
  for (size_t i = 0; i < str_len; i++) {
    if (str[i] == 'i' || str[i] == 'o' || str[i] == 'l') {
      return false;
    }
  }
  return true;
}

//Passwords must contain at least two different, non-overlapping pairs of letters, like aa, bb, or zz.
bool rule3_applies(char *str, size_t str_len) {
  char c = 0;
  size_t count = 0;
  for (size_t i = 0; i < str_len; i++) {
    if (str[i] == str[i + 1] && str[i] != c) {
      count++;
      c = str[i++];
    }
  }
  return count == 2;
}

void increment(char *str, size_t i) {
  if (str[i] == 'z') {
    str[i] = 'a';
    increment(str, i - 1);
  } else {
    str[i]++;
  }
}

void loop(char *str, size_t str_len, i64 *iterations) {
  while (true) {
    if (
      (
        rule1_applies(str, str_len) &&
        rule2_applies(str, str_len) &&
        rule3_applies(str, str_len)
      ) || str[0] == 'z'
    ) {
      break;
    }
    increment(str, str_len - 1);
    (*iterations)++;
  }
}

i64 part1(StringArray *lines) {
  i64 result = 0;

  string_array_foreach (char *line, lines) {
    size_t line_len = strlen(line);
    debug("%s: %zu\n", line, line_len);
    loop(line, line_len, &result);
    //printf("result = %s\n", line);
  }
  
  return result;
}

i64 part2(StringArray *lines) {
  i64 result = 0;

  string_array_foreach (char *line, lines) {
    size_t line_len = strlen(line);
    debug("%s: %zu\n", line, line_len);
    loop(line, line_len, &result);
    result++;
    increment(line, line_len - 1);
    loop(line, line_len, &result);
    //printf("result = %s\n", line);
  }
  
  return result;
}

int main(void) {
  test_case(day11, part1, data, 241148);
  test_case(day11, part2, data, 1191659);

  return 0;
}
