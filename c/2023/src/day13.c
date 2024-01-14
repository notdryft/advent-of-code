#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "string.h"

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

int part1(StringArray *lines) {
  //debugf(string_array_print_raw, lines);

  Array* patterns = array_new(int);
  array_push_rval(patterns, 0);
  for (size_t i = 0; i < lines->size; i++) {
    char *line = string_array_get(lines, i);
    if (strlen(line) == 0) {
      array_push_rval(patterns, i);
    }
  }
  array_push_rval(patterns, lines->size);
  debugf(int_array_print, patterns);

  int result = 0;
  for (size_t i = 0; i < patterns->size - 1; i++) {
    size_t start = int_array_get(patterns, i);
    start = i == 0 ? start : start + 1;
    size_t end = int_array_get(patterns, i + 1);

    size_t len = strlen(string_array_get(lines, start));
    int col = find_column_reflection(lines, len, start, end, -1);
    if (col != -1) {
      debug("pattern %zu: col %d\n", i + 1, col);
      result += col;
    }

    int row = find_row_reflection(lines, start, end, -1);
    if (row != -1) {
      debug("pattern %zu: row %d\n", i + 1, row);
      result += row * 100;
    }

#ifdef DEBUG
    if (col == -1 && row == -1) {
      debug("pattern %zu: nothing found?\n", i + 1);
    }
#endif
  }

  array_free(patterns);

  return result;
}

int part2(StringArray *lines) {
  //debugf(string_array_print_raw, lines);

  Array* patterns = array_new(int);
  array_push_rval(patterns, 0);
  for (size_t i = 0; i < lines->size; i++) {
    char *line = string_array_get(lines, i);
    if (strlen(line) == 0) {
      array_push_rval(patterns, i);
    }
  }
  array_push_rval(patterns, lines->size);
  //debugf(int_array_print, patterns);

  int result = 0;
  for (size_t i = 0; i < patterns->size - 1; i++) {
    size_t start = int_array_get(patterns, i);
    start = i == 0 ? start : start + 1;
    size_t end = int_array_get(patterns, i + 1);

    StringArray *dup = string_array_subset(lines, start, end);
    //debugf(string_array_print_raw, dup);

    int initial_col = find_column_reflection(dup, strlen(string_array_get(dup, 0)), 0, dup->size, -1);
#ifdef DEBUG
    if (initial_col != -1) {
      debug("pattern %zu: initial col %d\n", i + 1, initial_col);
    }
#endif

    int initial_row = find_row_reflection(dup, 0, dup->size, -1);
#ifdef DEBUG
    if (initial_row != -1) {
      debug("pattern %zu: initial row %d\n", i + 1, initial_row);
    }
    if (initial_col == -1 && initial_row == -1) {
      debug("pattern %zu: no initial col or row??\n", i + 1);
    }
#endif

    bool stop = false;
    for (size_t j = 0; j < dup->size && !stop; j++) {
      char *line = string_array_get(dup, j);
      for (size_t k = 0; k < strlen(line) && !stop; k++) {
        line[k] = line[k] == '.' ? '#' : '.';
        //debugf(string_array_print_raw, dup);
        int col = find_column_reflection(dup, strlen(line), 0, dup->size, initial_col);
        if (col != -1 && col != initial_col) {
          debug("pattern %zu: col %d by swapping (%zu, %zu)\n", i + 1, col, j, k);
          result += col;
          stop = true;
        }
        int row = find_row_reflection(dup, 0, dup->size, initial_row);
        debug("(%zu, %zu) -> %d\n", j, k, row);
        if (row != -1 && row != initial_row) {
          debug("pattern %zu: row %d by swapping (%zu, %zu)\n", i + 1, row, j, k);
          result += row * 100;
          stop = true;
        }
        line[k] = line[k] == '.' ? '#' : '.';
        //string_array_free(ddup);
      }
    }
    if (!stop) {
      debug("pattern %zu: no col or row?\n", i + 1);
    }
    string_array_free(dup);
  }

  array_free(patterns);

  return result;
}

int main(void) {
  test_case(day13, part1, sample, 405);
  test_case(day13, part1, data, 34821);
  test_case(day13, part2, sample, 400);
  test_case(day13, part2, data, 36919);

  return 0;
}
