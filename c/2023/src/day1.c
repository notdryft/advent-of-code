#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"

constexpr size_t BUFFER_LENGTH = 1024;

int part1(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == nullptr) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  int sum = 0;

  char buffer[BUFFER_LENGTH] = {};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    Array *digits = array_new(int);
    for (int i = 0; buffer[i] != '\n'; i++) {
      int value = buffer[i] - '0';
      if (value > 0 && value < 10) {
        array_push_rval(digits, value);
      }
    }
    sum += int_array_first(digits) * 10 + int_array_last(digits);
    array_free(digits);
  }

  fclose(fp);

  printf("sum = %d\n", sum);

  return sum;
}

int part2(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == nullptr) {
    fprintf(stderr, "Error: could not open file %s", filename);
    return 1;
  }

  int sum = 0;

  char buffer[BUFFER_LENGTH] = {};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    printf("%s", buffer);

    Array *digits = array_new(int);
    for (size_t i = 0; buffer[i] != '\n'; i++) {
      char c = buffer[i];
      if (c >= '1' && c <= '9') {
        array_push_rval(digits, c - '0');
      } else {
        if (strncmp(buffer + i, "one", 3) == 0) array_push_rval(digits, 1);
        else if (strncmp(buffer + i, "two", 3) == 0) array_push_rval(digits, 2);
        else if (strncmp(buffer + i, "three", 5) == 0) array_push_rval(digits, 3);
        else if (strncmp(buffer + i, "four", 4) == 0) array_push_rval(digits, 4);
        else if (strncmp(buffer + i, "five", 4) == 0) array_push_rval(digits, 5);
        else if (strncmp(buffer + i, "six", 3) == 0) array_push_rval(digits, 6);
        else if (strncmp(buffer + i, "seven", 5) == 0) array_push_rval(digits, 7);
        else if (strncmp(buffer + i, "eight", 5) == 0) array_push_rval(digits, 8);
        else if (strncmp(buffer + i, "nine", 4) == 0) array_push_rval(digits, 9);
      }
    }
    sum += int_array_first(digits) * 10 + int_array_last(digits);
    int_array_print(digits);
    array_free(digits);
  }

  fclose(fp);

  printf("sum = %d\n", sum);

  return sum;
}

int main(void) {
  assert(part1("../../inputs/2023/day1/sample1") == 142);
  assert(part1("../../inputs/2023/day1/data") == 55488);
  assert(part2("../../inputs/2023/day1/sample2") == 281);
  assert(part2("../../inputs/2023/day1/data") == 55614);

  return 0;
}
