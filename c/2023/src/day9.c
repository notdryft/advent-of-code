#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "string.h"

#define BUFFER_LENGTH 1024

int predict_first_rec(Array *array) {
  Array *sub = array_new(int);
  for (size_t i = 1; i < array->size; i++) {
    array_push(sub, int_array_get(array, i) - int_array_get(array, i - 1));
  }
  int_array_print(sub);
  if (int_array_all(sub, 0)) {
    int first = int_array_first(array);
    printf("array first = %d, sub array predirection = 0, prediction = %d\n", first, first);
    return first;
  } else {
    int sub_prediction = predict_first_rec(sub);
    array_free(sub);
    int first = int_array_first(array);
    int prediction = first - sub_prediction;
    printf("array first = %d, sub predirection = %d, prediction = %d\n", first, sub_prediction, prediction);
    return prediction;
  }
}

int predict_last_rec(Array *array) {
  Array *sub = array_new(int);
  for (size_t i = 1; i < array->size; i++) {
    array_push(sub, int_array_get(array, i) - int_array_get(array, i - 1));
  }
  int_array_print(sub);
  if (int_array_all(sub, 0)) {
    int last = int_array_last(array);
    printf("array last = %d, sub array predirection = 0, prediction = %d\n", last, last);
    return last;
  } else {
    int sub_prediction = predict_last_rec(sub);
    int last = int_array_last(array);
    int prediction = last + sub_prediction;
    printf("array last = %d, sub array predirection = %d, prediction = %d\n", last, sub_prediction, prediction);
    return prediction;
  }
}

int part1(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == nullptr) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  int sum = 0;

  char buffer[BUFFER_LENGTH] = {};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    StringArray *line_split = string_split(buffer, " ");
    Array *array = string_atoi(line_split);
    string_array_free(line_split);
    int_array_print(array);

    int last = predict_last_rec(array);
    array_free(array);
    sum += last;

    printf("sum = %d\n", sum);
  }

  fclose(fp);

  return sum;
}

int part2(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == nullptr) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  int sum = 0;

  char buffer[BUFFER_LENGTH] = {};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    StringArray *line_split = string_split(buffer, " ");
    Array *array = string_atoi(line_split);
    string_array_free(line_split);
    int_array_print(array);

    int first = predict_first_rec(array);
    sum += first;

    printf("sum = %d\n", sum);
  }

  fclose(fp);

  return sum;
}

int main(void) {
  assert(part1("../../inputs/2023/day9/sample") == 114);
  assert(part1("../../inputs/2023/day9/data") == 1842168671);
  assert(part2("../../inputs/2023/day9/sample") == 2);
  assert(part2("../../inputs/2023/day9/data") == 903);

  return 0;
}
