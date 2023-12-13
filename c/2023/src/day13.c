#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "string.h"

#define BUFFER_LENGTH 1024

int find_column_reflection(StringArray* lines, size_t len, size_t start, size_t end, int cmp) {
  for (size_t i = 1; i < len; i++) {
    size_t col = len - i < i ? len - i : i;
    //printf("min (%zu, %zu) = %zu\n", len, i, col);

    bool found_col = true;
    for (size_t j = 0; j < col && found_col; j++) {
      size_t left = i - j - 1;
      size_t right = i + j;
      //printf("comparing col %zu and %zu\n", left, right);

      bool stop = false;
      for (size_t k = start; k < end && !stop; k++) {
        char *line = string_array_get(lines, k);
        if (line[left] != line[right]) {
          stop = true;
        }
      }
      //printf("stop = %s\n", stop ? "true" : "false");
      found_col = found_col && !stop;
    }
    //printf("found = %s\n", found ? "true" : "false");
    //printf("\n");
    if (found_col) {
      int col = i;
      if (col != cmp) {
        return i;
      }
    }
  }
  return -1;
}

int find_row_reflection(StringArray *lines, size_t start, size_t end, int cmp) {
  for (size_t i = start + 1; i < end; i++) {
    size_t row = end - i < i - start ? end - i : i - start;
    //printf("min (end - i = %zu, start - i = %zu) = %zu\n", end - i , i - start, row);

    bool stop = false;
    for (size_t j = 0; j < row && !stop; j++) {
      size_t top = i - j - 1;
      size_t bottom = i + j;
      //printf("comparing row %zu and %zu\n", top, bottom);

      char *line_top = string_array_get(lines, top);
      char *line_bottom = string_array_get(lines, bottom);
      if (strcmp(line_top, line_bottom) != 0) {
        stop = true;
      }
    }
    if (!stop) {
      int row = i - start;
      if (row != cmp) {
        return i - start;
      }
    }
  }
  return -1;
}

int part1(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  StringArray *lines = string_array_new();

  char buffer[BUFFER_LENGTH] = {0};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';
    string_array_push(lines, buffer);
  }

  fclose(fp);

  //string_array_print_raw(lines);
  Array* patterns = array_new(int);
  array_push(patterns, 0);
  for (size_t i = 0; i < lines->size; i++) {
    char *line = string_array_get(lines, i);
    if (strlen(line) == 0) {
      array_push(patterns, i);
    }
  }
  array_push(patterns, lines->size);
  int_array_print(patterns);

  int sum = 0;

  for (size_t i = 0; i < patterns->size - 1; i++) {
    size_t start = int_array_get(patterns, i);
    start = i == 0 ? start : start + 1;
    size_t end = int_array_get(patterns, i + 1);

    size_t len = strlen(string_array_get(lines, start));
    int col = find_column_reflection(lines, len, start, end, -1);
    if (col != -1) {
      printf("pattern %zu: col %d\n", i + 1, col);
      sum += col;
    }

    int row = find_row_reflection(lines, start, end, -1);
    if (row != -1) {
      printf("pattern %zu: row %d\n", i + 1, row);
      sum += row * 100;
    }

    if (col == -1 && row == -1) {
      printf("pattern %zu: nothing found?\n", i + 1);
    }
    //printf("\n");
  }

  array_free(patterns);
  string_array_free(lines);

  printf("%d\n", sum);

  return sum;
}

int part2(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  StringArray *lines = string_array_new();

  char buffer[BUFFER_LENGTH] = {0};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';
    string_array_push(lines, buffer);
  }

  fclose(fp);

  //string_array_print_raw(lines);
  Array* patterns = array_new(int);
  array_push(patterns, 0);
  for (size_t i = 0; i < lines->size; i++) {
    char *line = string_array_get(lines, i);
    if (strlen(line) == 0) {
      array_push(patterns, i);
    }
  }
  array_push(patterns, lines->size);
  //int_array_print(patterns);

  int sum = 0;

  for (size_t i = 0; i < patterns->size - 1; i++) {
    size_t start = int_array_get(patterns, i);
    start = i == 0 ? start : start + 1;
    size_t end = int_array_get(patterns, i + 1);

    StringArray *dup = string_array_subset(lines, start, end);
    //string_array_print_raw(dup);

    int initial_col = find_column_reflection(dup, strlen(string_array_get(dup, 0)), 0, dup->size, -1);
    if (initial_col != -1) {
      printf("pattern %zu: initial col %d\n", i + 1, initial_col);
    }

    int initial_row = find_row_reflection(dup, 0, dup->size, -1);
    if (initial_row != -1) {
      printf("pattern %zu: initial row %d\n", i + 1, initial_row);
    }

    if (initial_col == -1 && initial_row == -1) {
      printf("pattern %zu: no initial col or row??\n", i + 1);
    }

    bool stop = false;
    for (size_t j = 0; j < dup->size && !stop; j++) {
      char *line = string_array_get(dup, j);
      for (size_t k = 0; k < strlen(line) && !stop; k++) {
        line[k] = line[k] == '.' ? '#' : '.';
        //string_array_print_raw(dup);
        //printf("\n");
        int col = find_column_reflection(dup, strlen(line), 0, dup->size, initial_col);
        if (col != -1 && col != initial_col) {
          printf("pattern %zu: col %d by swapping (%zu, %zu)\n", i + 1, col, j, k);
          sum += col;
          stop = true;
        }
        int row = find_row_reflection(dup, 0, dup->size, initial_row);
        //printf("(%zu, %zu) -> %d\n", j, k, row);
        if (row != -1 && row != initial_row) {
          printf("pattern %zu: row %d by swapping (%zu, %zu)\n", i + 1, row, j, k);
          sum += row * 100;
          stop = true;
        }
        line[k] = line[k] == '.' ? '#' : '.';
        //string_array_free(ddup);
      }
    }
    if (!stop) {
      printf("pattern %zu: no col or row?\n", i + 1);
    }
    string_array_free(dup);
  }

  array_free(patterns);
  string_array_free(lines);

  printf("%d\n", sum);

  return sum;
}

int main() {
  assert(part1("../../inputs/2023/day13/part1_test") == 405);
  assert(part1("../../inputs/2023/day13/data") == 34821);
  assert(part2("../../inputs/2023/day13/part2_test") == 400);
  assert(part2("../../inputs/2023/day13/data") == 36919);

  return 0;
}
