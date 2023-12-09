#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string.h"

const unsigned int BUFFER_LENGTH = 1024;

void int_array_print(int *array, size_t size) {
  printf("Array{ size = %zu, data = [", size);
  for (size_t i = 0; i < size; i++) {
    int item = array[i];
    if (i == size - 1) {
      printf(" %d", item);
    } else {
      printf(" %d,", item);
    }
  }
  printf(" ] }\n");
}

bool int_array_all(int *array, size_t size, int expected) {
  for (size_t i = 0; i < size; i++) {
    if (array[i] != expected) {
      return false;
    }
  }
  return true;
}

int predict_first_rec(int *array, size_t size) {
  int sub_array_size = 0;
  int sub_array[1000] = {0};
  for (size_t i = 1; i < size; i++) {
    sub_array[sub_array_size++] = array[i] - array[i - 1];
  }
  int_array_print(sub_array, sub_array_size);
  if (int_array_all(sub_array, sub_array_size, 0)) {
    printf("array first = %d, sub array predirection = 0, prediction = %d\n", array[0], array[0]);
    return array[0];
  } else {
    int sub_array_prediction = predict_first_rec(sub_array, sub_array_size);
    int prediction = array[0] - sub_array_prediction;
    printf("array first = %d, sub array predirection = %d, prediction = %d\n", array[0], sub_array_prediction, prediction);
    return prediction;
  }
}

int predict_last_rec(int *array, size_t size) {
  int sub_array_size = 0;
  int sub_array[1000] = {0};
  for (size_t i = 1; i < size; i++) {
    sub_array[sub_array_size++] = array[i] - array[i - 1];
  }
  int_array_print(sub_array, sub_array_size);
  if (int_array_all(sub_array, sub_array_size, 0)) {
    printf("array last = %d, sub array predirection = 0, prediction = %d\n", array[size - 1], array[size - 1]);
    return array[size - 1];
  } else {
    int sub_array_prediction = predict_last_rec(sub_array, sub_array_size);
    int prediction = array[size - 1] + sub_array_prediction;
    printf("array last = %d, sub array predirection = %d, prediction = %d\n", array[size - 1], sub_array_prediction, prediction);
    return prediction;
  }
}

int part1(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  int sum = 0;

  char buffer[BUFFER_LENGTH];
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    StringArray *line_split = string_split(buffer, " ");

    size_t array_size = 0;
    int array[1000] = {0};

    for (size_t i = 0; i < line_split->size; i++) {
      array[array_size++] = atoi(string_array_get(line_split, i));
    }
    string_array_free(line_split);

    int_array_print(array, array_size);
    int last = predict_last_rec(array, array_size);

    sum += last;
    printf("sum = %d\n", sum);
  }

  fclose(fp);

  return sum;
}

int part2(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  int sum = 0;

  char buffer[BUFFER_LENGTH];
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    StringArray *line_split = string_split(buffer, " ");

    size_t array_size = 0;
    int array[1000] = {0};

    for (size_t i = 0; i < line_split->size; i++) {
      array[array_size++] = atoi(string_array_get(line_split, i));
    }
    string_array_free(line_split);

    int_array_print(array, array_size);
    int first = predict_first_rec(array, array_size);

    sum += first;
    printf("sum = %d\n", sum);
  }

  fclose(fp);

  return sum;
}

int main() {
  assert(part1("../../inputs/2023/day9/part1_test") == 114);
  assert(part1("../../inputs/2023/day9/data") == 1842168671);
  assert(part2("../../inputs/2023/day9/part2_test") == 2);
  assert(part2("../../inputs/2023/day9/data") == 903);

  return 0;
}
