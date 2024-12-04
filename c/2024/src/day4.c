#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

i64 part1(StringArray *lines) {
  size_t cols = strlen(string_array_get(lines, 0));
  size_t rows = lines->size;

  tracef(string_array_print_raw, lines);
  trace("\n");

  i64 result = 0;

  char *xmas = "XMAS";
  char *reverse = "SAMX";

  char **grid = malloc(sizeof(*grid) * rows);
  for (size_t j = 0; j < rows; j++) {
    grid[j] = calloc(cols, sizeof(**grid));
    for (size_t i = 0; i < cols; i++) {
      grid[j][i] = '.';
    }
  }

  for (size_t j = 0; j < rows; j++) {
    char *row = string_array_get(lines, j);
    for (size_t i = 0; i < cols; i++) {
      if (row[i] == xmas[0]) {
        if (j + 4 <= rows) {
          bool perpendicular = true;
          for (size_t k = 1; k < 4; k++) {
            char *jkrow = string_array_get(lines, j + k);
            perpendicular = perpendicular && jkrow[i] == xmas[k];
          }
          if (perpendicular) {
            debug("perpendicular at (%zu, %zu)\n", i, j);
            for (size_t k = 0; k < 4; k++) {
              grid[j + k][i] = xmas[k];
            }
            result++;
          }
        }
        if (i + 4 <= cols) {
          bool straight = true;
          for (size_t k = 1; k < 4; k++) {
            straight = straight && row[i + k] == xmas[k];
          }
          if (straight) {
            debug("straight at (%zu, %zu)\n", i, j);
            for (size_t k = 0; k < 4; k++) {
              grid[j][i + k] = xmas[k];
            }
            result++;
          }
          if (j + 4 <= rows) {
            bool diagonal_down_right = true;
            for (size_t k = 1; k < 4; k++) {
              char *jkrow = string_array_get(lines, j + k);
              diagonal_down_right = diagonal_down_right && jkrow[i + k] == xmas[k];
            }
            if (diagonal_down_right) {
              debug("diagonal down right at (%zu, %zu)\n", i, j);
              for (size_t k = 0; k < 4; k++) {
                grid[j + k][i + k] = xmas[k];
              }
              result++;
            }
          }
        }
        if (i >= 3 && j + 4 <= rows) {
          bool diagonal_down_left = true;
          for (size_t k = 1; k < 4; k++) {
            char *jkrow = string_array_get(lines, j + k);
            diagonal_down_left = diagonal_down_left && jkrow[i - k] == xmas[k];
          }
          if (diagonal_down_left) {
            debug("diagonal down left at (%zu, %zu)\n", i, j);
            for (size_t k = 0; k < 4; k++) {
              grid[j + k][i - k] = xmas[k];
            }
            result++;
          }
        }
      } else if (row[i] == reverse[0]) {
        if (j + 4 <= rows) {
          bool perpendicular = true;
          for (size_t k = 1; k < 4; k++) {
            char *jkrow = string_array_get(lines, j + k);
            perpendicular = perpendicular && jkrow[i] == reverse[k];
          }
          if (perpendicular) {
            debug("perpendicular at (%zu, %zu)\n", i, j);
            for (size_t k = 0; k < 4; k++) {
              grid[j + k][i] = reverse[k];
            }
            result++;
          }
        }
        if (i + 4 <= cols) {
          bool straight = true;
          for (size_t k = 1; k < 4; k++) {
            straight = straight && row[i + k] == reverse[k];
          }
          if (straight) {
            debug("straight at (%zu, %zu)\n", i, j);
            for (size_t k = 0; k < 4; k++) {
              grid[j][i + k] = reverse[k];
            }
            result++;
          }
          if (j + 4 <= rows) {
            bool diagonal_down_right = true;
            for (size_t k = 1; k < 4; k++) {
              char *jkrow = string_array_get(lines, j + k);
              diagonal_down_right = diagonal_down_right && jkrow[i + k] == reverse[k];
            }
            if (diagonal_down_right) {
              debug("diagonal down right at (%zu, %zu)\n", i, j);
              for (size_t k = 0; k < 4; k++) {
                grid[j + k][i + k] = reverse[k];
              }
              result++;
            }
          }
        }
        if (i >= 3 && j + 4 <= rows) {
          bool diagonal_down_left = true;
          for (size_t k = 1; k < 4; k++) {
            char *jkrow = string_array_get(lines, j + k);
            diagonal_down_left = diagonal_down_left && jkrow[i - k] == reverse[k];
          }
          if (diagonal_down_left) {
            debug("diagonal down left at (%zu, %zu)\n", i, j);
            for (size_t k = 0; k < 4; k++) {
              grid[j + k][i - k] = reverse[k];
            }
            result++;
          }
        }
      }
    }
  }

#ifdef DEBUG
  for (size_t j = 0; j < rows; j++) {
    for (size_t i = 0; i < cols; i++) {
      printf("%c", grid[j][i]);
    }
    printf("\n");
  }
#endif

  for (size_t j = 0; j < cols; j++) {
    free(grid[j]);
  }
  free(grid);

  return result;
}

i64 part2(StringArray *lines) {
  size_t cols = strlen(string_array_get(lines, 0));
  size_t rows = lines->size;

  tracef(string_array_print_raw, lines);
  trace("\n");

  i64 result = 0;

  char **grid = malloc(sizeof(*grid) * rows);
  for (size_t j = 0; j < rows; j++) {
    grid[j] = calloc(cols, sizeof(**grid));
    for (size_t i = 0; i < cols; i++) {
      grid[j][i] = '.';
    }
  }

  for (size_t j = 1; j < rows - 1; j++) {
    char *prow = string_array_get(lines, j - 1);
    char *row = string_array_get(lines, j);
    char *nrow = string_array_get(lines, j + 1);
    for (size_t i = 1; i < cols - 1; i++) {
      if (row[i] == 'A') {
        bool mas = true;
        mas = mas && ((prow[i - 1] == 'M' && nrow[i + 1] == 'S') || (prow[i - 1] == 'S' && nrow[i + 1] == 'M'));
        mas = mas && ((prow[i + 1] == 'M' && nrow[i - 1] == 'S') || (prow[i + 1] == 'S' && nrow[i - 1] == 'M'));
        if (mas) {
          result++;
        }
      }
    }
  }

#ifdef DEBUG
  for (size_t j = 0; j < rows; j++) {
    for (size_t i = 0; i < cols; i++) {
      printf("%c", grid[j][i]);
    }
    printf("\n");
  }
#endif

  for (size_t j = 0; j < cols; j++) {
    free(grid[j]);
  }
  free(grid);

  return result;
}

int main(void) {
  test_case(day4, part1, sample, 18);
  test_case(day4, part1, data, 2517);
  test_case(day4, part2, sample, 9);
  test_case(day4, part2, data, 1960);

  return 0;
}
