#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

i64 neighbours(char **grid, size_t cols, size_t rows, size_t x, size_t y) {
  size_t sj = y == 0 ? 0 : y - 1;
  size_t ej = y == rows - 1 ? rows - 1 : y + 1;
  size_t si = x == 0 ? 0 : x - 1;
  size_t ei = x == cols - 1 ? cols - 1 : x + 1;

  i64 result = 0;
  for (size_t j = sj; j <= ej; j++) {
    for (size_t i = si; i <= ei; i++) {
      if (i != x || j != y) {
        trace("(%zu, %zu) = %c\n", j, i, grid[j][i]);
        if (grid[j][i] == '#') {
          result++;
        }
      }
    }
  }
  debug("(%zu, %zu) -> (%zu, %zu) to (%zu, %zu) = %lld\n", y, x, sj, si, ej, ei, result);

  return result;
}

i64 alive(char **grid, size_t cols, size_t rows) {
  i64 result = 0;
  for (size_t j = 0; j < rows; j++) {
    for (size_t i = 0; i < cols; i++) {
      if (grid[j][i] == '#') {
        result++;
      }
    }
  }

  return result;
}

i64 part1(StringArray *lines, size_t steps) {
  size_t cols = strlen(string_array_get(lines, 0));
  size_t rows = lines->size;
  tracef(string_array_print_raw, lines);
  trace("\n");

  StringArray *grid = string_array_dup(lines);
  for (size_t step = 0; step < steps; step++) {
    debug("step %zu:\n", step);
    StringArray *tmp = string_array_dup(grid);

    for (size_t j = 0; j < rows; j++) {
      for (size_t i = 0; i < cols; i++) {
        i64 n = neighbours(grid->items, cols, rows, i, j);
        if (grid->items[j][i] == '#') {
          if (n != 2 && n != 3) {
            tmp->items[j][i] = '.';
          }
        } else {
          if (n == 3) {
            tmp->items[j][i] = '#';
          }
        }
      }
    }
    string_array_free(grid);
    grid = tmp;

    tracef(string_array_print_raw, grid);
    trace("\n");
  }

  i64 result = alive(grid->items, cols, rows);
  string_array_free(grid);

  return result;
}

i64 part2(StringArray *lines, size_t steps) {
  size_t cols = strlen(string_array_get(lines, 0));
  size_t rows = lines->size;
  tracef(string_array_print_raw, lines);
  trace("\n");

  StringArray *grid = string_array_dup(lines);
  grid->items[0][0] = '#';
  grid->items[0][cols - 1] = '#';
  grid->items[rows - 1][0] = '#';
  grid->items[rows - 1][cols - 1] = '#';

  for (size_t step = 0; step < steps; step++) {
    debug("step %zu:\n", step);
    StringArray *tmp = string_array_dup(grid);

    for (size_t j = 0; j < rows; j++) {
      for (size_t i = 0; i < cols; i++) {
        i64 n = neighbours(grid->items, cols, rows, i, j);
        if (
          !(
            (j == 0 && i == 0) ||
            (j == 0 && i == cols - 1) ||
            (j == rows - 1 && i == 0) ||
            (j == rows - 1 && i == cols - 1)
          )
        ) {
          if (grid->items[j][i] == '#') {
            if (n != 2 && n != 3) {
              tmp->items[j][i] = '.';
            }
          } else {
            if (n == 3) {
              tmp->items[j][i] = '#';
            }
          }
        }
      }
    }
    string_array_free(grid);
    grid = tmp;

    tracef(string_array_print_raw, grid);
    trace("\n");
  }

  i64 result = alive(grid->items, cols, rows);
  string_array_free(grid);

  return result;
}

int main(void) {
  test_case(day18, part1, sample, 4, 4);
  test_case(day18, part1, data, 1061, 100);
  test_case(day18, part2, sample, 17, 5);
  test_case(day18, part2, data, 1006, 100);

  return 0;
}
