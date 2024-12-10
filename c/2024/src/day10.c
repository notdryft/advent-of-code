#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

void reset(bool **seen, size_t cols, size_t rows) {
  for (size_t j = 0; j < rows; j++) {
    for (size_t i = 0; i < cols; i++) {
      seen[j][i] = false;
    }
  }
}

i64 traverse_rec(char **grid, bool **seen, size_t cols, size_t rows, size_t i, size_t j) {
  if (grid[j][i] == '9' && !seen[j][i]) {
    seen[j][i] = true;
    return 1;
  }

  i64 result = 0;
  char next = grid[j][i] + 1;

  if (j > 0 && grid[j - 1][i] == next) {
    result += traverse_rec(grid, seen, cols, rows, i, j - 1);
  }
  if (i < cols - 1 && grid[j][i + 1] == next) {
    result += traverse_rec(grid, seen, cols, rows, i + 1, j);
  }
  if (j < rows - 1 && grid[j + 1][i] == next) {
    result += traverse_rec(grid, seen, cols, rows, i, j + 1);
  }
  if (i > 0 && grid[j][i - 1] == next) {
    result += traverse_rec(grid, seen, cols, rows, i - 1, j);
  }

   return result;
}

i64 part1(StringArray *lines) {
  size_t cols = strlen(string_array_get(lines, 0));
  size_t rows = lines->size;
  char **grid = lines->items;

  tracef(string_array_print_raw, lines);
  trace("\n");
  i64 result = 0;

  bool **seen = malloc(sizeof(*seen) * rows);
  for (size_t j = 0; j < rows; j++) {
    seen[j] = calloc(cols, sizeof(**seen));
  }

  for (size_t j = 0; j < rows; j++) {
    for (size_t i = 0; i < cols; i++) {
      if (grid[j][i] == '0') {
        reset(seen, cols, rows);
        result += traverse_rec(grid, seen, cols, rows, i, j);
      }
    }
  }

  for (size_t j = 0; j < rows; j++) {
    free(seen[j]);
  }
  free(seen);

  return result;
}

i64 traverse_rec2(char **grid, size_t cols, size_t rows, size_t i, size_t j) {
  if (grid[j][i] == '9') {
    return 1;
  }

  i64 result = 0;
  char next = grid[j][i] + 1;

  if (j > 0 && grid[j - 1][i] == next) {
    result += traverse_rec2(grid, cols, rows, i, j - 1);
  }
  if (i < cols - 1 && grid[j][i + 1] == next) {
    result += traverse_rec2(grid, cols, rows, i + 1, j);
  }
  if (j < rows - 1 && grid[j + 1][i] == next) {
    result += traverse_rec2(grid, cols, rows, i, j + 1);
  }
  if (i > 0 && grid[j][i - 1] == next) {
    result += traverse_rec2(grid, cols, rows, i - 1, j);
  }

   return result;
}

i64 part2(StringArray *lines) {
  size_t cols = strlen(string_array_get(lines, 0));
  size_t rows = lines->size;
  char **grid = lines->items;

  tracef(string_array_print_raw, lines);
  trace("\n");
  i64 result = 0;

  for (size_t j = 0; j < rows; j++) {
    for (size_t i = 0; i < cols; i++) {
      if (grid[j][i] == '0') {
        result += traverse_rec2(grid, cols, rows, i, j);
      }
    }
  }

  return result;
}

int main(void) {
  test_case(day10, part1, sample, 36);
  test_case(day10, part1, data, 535);
  test_case(day10, part2, sample, 81);
  test_case(day10, part2, data, 1186);

  return 0;
}
