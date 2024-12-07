#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

bool compute2_rec(i64 number, i64 acc, Array *numbers, size_t i) {
  if (i == numbers->size) {
    return number == acc;
  }
  i64 next = i64_array_get(numbers, i);
  return compute2_rec(number, acc + next, numbers, i + 1) ||
         compute2_rec(number, acc * next, numbers, i + 1);
}

bool compute2(i64 number, Array *numbers) {
  return compute2_rec(number, i64_array_get(numbers, 0), numbers, 1);
}

i64 part1(StringArray *lines) {
  i64 result = 0;

  string_array_foreach (char *line, lines) {
    StringArray *csplit = string_split(line, ": ");
    i64 number = atoll(string_array_get(csplit, 0));

    StringArray *ssplit = string_split(string_array_get(csplit, 1), " ");
    Array *numbers = string_atoll(ssplit);

    if (compute2(number, numbers)) {
      result += number;
    }

    array_free(numbers);
    string_array_free(csplit);
    string_array_free(ssplit);
  }

  return result;
}

bool compute3_rec(i64 number, i64 acc, Array *numbers, size_t i) {
  if (acc > number) {
    return false;
  } else if (i == numbers->size) {
    return number == acc;
  }
  i64 next = i64_array_get(numbers, i);

  char buffer[100] = {};
  sprintf(buffer, "%lld%lld", acc, next);
  i64 concat = atoll(buffer);

  return compute3_rec(number, concat, numbers, i + 1) ||
         compute3_rec(number, acc + next, numbers, i + 1) ||
         compute3_rec(number, acc * next, numbers, i + 1);
}

bool compute3(i64 number, Array *numbers) {
  return compute3_rec(number, i64_array_get(numbers, 0), numbers, 1);
}

i64 part2(StringArray *lines) {
  i64 result = 0;

  string_array_foreach (char *line, lines) {
    StringArray *csplit = string_split(line, ": ");
    i64 number = atoll(string_array_get(csplit, 0));

    StringArray *ssplit = string_split(string_array_get(csplit, 1), " ");
    Array *numbers = string_atoll(ssplit);

    if (compute3(number, numbers)) {
      result += number;
    }

    array_free(numbers);
    string_array_free(csplit);
    string_array_free(ssplit);
  }

  return result;
}

int main(void) {
  test_case(day7, part1, sample, 3749);
  test_case(day7, part1, data, 3351424677624);
  test_case(day7, part2, sample, 11387);
  test_case(day7, part2, data, 204976636995111);

  return 0;
}
