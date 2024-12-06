#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

#define RIGHT 1
#define DOWN 2
#define LEFT 4
#define UP 8

i64 traverse(char **grid, char **mask, size_t cols, size_t rows, size_t si, size_t sj) {
  size_t ci = si;
  size_t cj = sj;
  char direction = UP;

  i64 distinct = 0;

  bool oob = false;
  while (!oob) {
    if (mask != nullptr) {
      if ((mask[cj][ci] & direction) != 0) {
        return -1;
      }
      mask[cj][ci] |= direction;
    }
    if (direction == UP && cj > 0 && grid[cj - 1][ci] == '#') {
      direction = RIGHT;
    } else if (direction == RIGHT && ci < cols - 1 && grid[cj][ci + 1] == '#') {
      direction = DOWN;
    } else if (direction == DOWN && cj < rows - 1 && grid[cj + 1][ci] == '#') {
      direction = LEFT;
    } else if (direction == LEFT && ci > 0 && grid[cj][ci - 1] == '#') {
      direction = UP;
    }
    if (direction == UP && cj > 0 && grid[cj - 1][ci] == '#') {
      direction = RIGHT;
    } else if (direction == RIGHT && ci < cols - 1 && grid[cj][ci + 1] == '#') {
      direction = DOWN;
    } else if (direction == DOWN && cj < rows - 1 && grid[cj + 1][ci] == '#') {
      direction = LEFT;
    } else if (direction == LEFT && ci > 0 && grid[cj][ci - 1] == '#') {
      direction = UP;
    }
    if (grid[cj][ci] == '.') {
      distinct++;
      grid[cj][ci] = 'X';
    }
    if (direction == UP) {
      if (cj > 0) cj--;
      else oob = true;
    } else if (direction == RIGHT) {
      if (ci < cols - 1) ci++;
      else oob = true;
    } else if (direction == DOWN) {
      if (cj < rows - 1) cj++;
      else oob = true;
    } else {
      if (ci > 0) ci--;
      else oob = true;
    }
  }

  return distinct;
}

i64 part1(StringArray *lines) {
  size_t cols = strlen(string_array_get(lines, 0));
  size_t rows = lines->size;

  tracef(string_array_print_raw, lines);
  trace("\n");

  size_t si = 0;
  size_t sj = 0;
  for (size_t j = 0; j < rows; j++) {
    char *row = string_array_get(lines, j);
    for (size_t i = 0; i < cols; i++) {
      if (row[i] == '^') {
        si = i;
        sj = j;
      }
    }
  }
  debug("start: %zu, %zu\n", si, sj);
  lines->items[sj][si] = '.';

  i64 result = traverse(lines->items, nullptr, cols, rows, si, sj);

  tracef(string_array_print_raw, lines);
  trace("\n");

  return result;
}


i64 part2(StringArray *lines) {
  size_t cols = strlen(string_array_get(lines, 0));
  size_t rows = lines->size;

  tracef(string_array_print_raw, lines);
  trace("\n");
  i64 result = 0;

  size_t si = 0;
  size_t sj = 0;
  for (size_t j = 0; j < rows; j++) {
    char *row = string_array_get(lines, j);
    for (size_t i = 0; i < cols; i++) {
      if (row[i] == '^') {
        si = i;
        sj = j;
      }
    }
  }
  debug("start: %zu, %zu\n", si, sj);
  lines->items[sj][si] = '.';

  StringArray *base = string_array_dup(lines);
  traverse(base->items, nullptr, cols, rows, si, sj);
  tracef(string_array_print_raw, base);
  trace("\n");

  char **mask = malloc(sizeof(*mask) * rows);
  for (size_t k = 0; k < rows; k++) {
    mask[k] = calloc(cols, sizeof(**mask));
  }

  for (size_t j = 0; j < rows; j++) {
    for (size_t i = 0; i < cols; i++) {
      debug("%zu %zu (%c)\n", i, j, base->items[j][i]);
      if ((i != si || j != sj) && base->items[j][i] == 'X') {
        StringArray *dup = string_array_dup(lines);
        dup->items[j][i] = '#';

        for (size_t jj = 0; jj < rows; jj++) {
          memset(mask[jj], 0, sizeof(**mask) * cols);
        }

        i64 distinct = traverse(dup->items, mask, cols, rows, si, sj);
        if (distinct == -1) {
          trace("obstruction at (%zu, %zu):\n", i, j);
          dup->items[sj][si] = '^';
          dup->items[j][i] = 'O';
          tracef(string_array_print_raw, dup);
          trace("\n");

          result++;
        }

        string_array_free(dup);
      }
    }
  }

  for (size_t i = 0; i < rows; i++) {
    free(mask[i]);
  }
  free(mask);
  string_array_free(base);

  return result;
}

int main(void) {
  test_case(day6, part1, sample, 41);
  test_case(day6, part1, data, 5153);
  test_case(day6, part2, sample, 6);
  test_case(day6, part2, data, 1711);

  return 0;
}
