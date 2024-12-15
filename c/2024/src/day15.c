#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

void print_nrows(StringArray *lines, size_t rows) {
  for (size_t j = 0; j < rows; j++) {
    printf("%s\n", lines->items[j]);
  }
  printf("\n");
}

void move_part1(char **grid, size_t *x, size_t *y, char instruction) {
  if (instruction == '^') {
    char next = grid[*y - 1][*x];
    if (next == '.') {
      grid[(*y)--][*x] = '.';
      grid[*y][*x] = '@';
    } else if (next == 'O') {
      size_t ny = *y - 1;
      while (grid[ny][*x] == 'O') {
        ny--;
      }
      if (grid[ny][*x] == '.') {
        grid[(*y)--][*x] = '.';
        grid[*y][*x] = '@';
        grid[ny][*x] = 'O';
      }
    }
  } else if (instruction == '>') {
    char next = grid[*y][*x + 1];
    if (next == '.') {
      grid[*y][(*x)++] = '.';
      grid[*y][*x] = '@';
    } else if (next == 'O') {
      size_t nx = *x + 1;
      while (grid[*y][nx] == 'O') {
        nx++;
      }
      if (grid[*y][nx] == '.') {
        grid[*y][(*x)++] = '.';
        grid[*y][*x] = '@';
        grid[*y][nx] = 'O';
      }
    }
  } else if (instruction == 'v') {
    char next = grid[*y + 1][*x];
    if (next == '.') {
      grid[(*y)++][*x] = '.';
      grid[*y][*x] = '@';
    } else if (next == 'O') {
      size_t ny = *y + 1;
      while (grid[ny][*x] == 'O') {
        ny++;
      }
      if (grid[ny][*x] == '.') {
        grid[(*y)++][*x] = '.';
        grid[*y][*x] = '@';
        grid[ny][*x] = 'O';
      }
    }
  } else if (instruction == '<') {
    char next = grid[*y][*x - 1];
    if (next == '.') {
      grid[*y][(*x)--] = '.';
      grid[*y][*x] = '@';
    } else if (next == 'O') {
      size_t nx = *x - 1;
      while (grid[*y][nx] == 'O') {
        nx--;
      }
      if (grid[*y][nx] == '.') {
        grid[*y][(*x)--] = '.';
        grid[*y][*x] = '@';
        grid[*y][nx] = 'O';
      }
    }
  }
}

i64 part1(StringArray *lines) {
  size_t cols = strlen(string_array_get(lines, 0));
  size_t rows = 0;

  size_t x = 1, y = 1;
  string_array_enumerate (char *line, j, lines) {
    size_t line_len = strlen(line);
    if (line_len > 0) {
      if (line[0] == '#') {
        rows++;
        for (size_t i = 0; i < cols; i++) {
          if (line[i] == '@') {
            x = i;
            y = j;
            debug("robot found at (%zu, %zu)\n", x, y);
          }
        }
      } else {
        for (size_t i = 0; i < line_len; i++) {
          move_part1(lines->items, &x, &y, line[i]);
        }
      }
    } else {
      debugf(print_nrows, lines, rows);
      debug("grid is %zux%zu\n", cols, rows);
    }
  }
  debugf(print_nrows, lines, rows);

  i64 result = 0;
  for (size_t j = 0; j < rows; j++) {
    for (size_t i = 0; i < cols; i++) {
      if (lines->items[j][i] == 'O') {
        result += 100 * j + i;
      }
    }
  }

  return result;
}

typedef struct {
  size_t x;
  size_t y;
} Vec2;

bool array_contains(Array *array, Vec2 *v) {
  array_foreach(Vec2 *u, array) {
    if (u->x == v->x && u->y == v->y) {
      return true;
    }
  }
  return false;
}

void swap(char **grid, size_t x1, size_t y1, size_t x2, size_t y2) {
  char tmp = grid[y2][x2];
  grid[y2][x2] = grid[y1][x1];
  grid[y1][x1] = tmp;
}

bool move_blocks_north(char **grid, size_t sx, size_t sy) {
  Array *q = array_new(Vec2);
  array_push(q, &(Vec2) { .x = sx, .y = sy });

  Array *seen = array_new(Vec2);
  while (q->size > 0) {
    Vec2 *v = array_shift(q);
    if (array_contains(seen, v)) {
      free(v);
      continue;
    }
    array_push(seen, v);

    size_t x = v->x, y = v->y;
    if (grid[y - 1][x] == '#' || grid[y - 1][x + 1] == '#') {
      array_free(q);
      array_free(seen);
      free(v);

      return false;
    }

    if (grid[y - 1][x] == ']') {
      array_push(q, &(Vec2) { .x = x - 1, .y = y - 1 });
    }
    if (grid[y - 1][x] == '[') {
      array_push(q, &(Vec2) { .x = x, .y = y - 1 });
    }
    if (grid[y - 1][x + 1] == '[') {
      array_push(q, &(Vec2) { .x = x + 1, .y = y - 1 });
    }

    free(v);
  }

  while (seen->size > 0) {
    Vec2 *v = array_pop(seen);
    swap(grid, v->x, v->y, v->x, v->y - 1);
    swap(grid, v->x + 1, v->y, v->x + 1, v->y - 1);

    free(v);
  }

  array_free(seen);
  array_free(q);

  return true;
}

bool move_blocks_south(char **grid, size_t sx, size_t sy) {
  Array *q = array_new(Vec2);
  array_push(q, &(Vec2) { .x = sx, .y = sy });

  Array *seen = array_new(Vec2);
  while (q->size > 0) {
    Vec2 *v = array_shift(q);
    if (array_contains(seen, v)) {
      free(v);
      continue;
    }
    array_push(seen, v);

    size_t x = v->x, y = v->y;
    if (grid[y + 1][x] == '#' || grid[y + 1][x + 1] == '#') {
      array_free(q);
      array_free(seen);
      free(v);

      return false;
    }

    if (grid[y + 1][x] == ']') {
      array_push(q, &(Vec2) { .x = x - 1, .y = y + 1 });
    }
    if (grid[y + 1][x] == '[') {
      array_push(q, &(Vec2) { .x = x, .y = y + 1 });
    }
    if (grid[y + 1][x + 1] == '[') {
      array_push(q, &(Vec2) { .x = x + 1, .y = y + 1 });
    }

    free(v);
  }

  while (seen->size > 0) {
    Vec2 *v = array_pop(seen);
    swap(grid, v->x, v->y, v->x, v->y + 1);
    swap(grid, v->x + 1, v->y, v->x + 1, v->y + 1);

    free(v);
  }

  array_free(seen);
  array_free(q);

  return true;
}

void move_part2(char **grid, size_t *x, size_t *y, char instruction) {
  if (instruction == '^') {
    char next = grid[*y - 1][*x];
    if (next == '.') {
      grid[(*y)--][*x] = '.';
      grid[*y][*x] = '@';
    } else if (next == '[') {
      if (move_blocks_north(grid, *x, *y - 1)) {
        swap(grid, *x, *y, *x, *y - 1);
        (*y)--;
      }
    } else if (next == ']') {
      if (move_blocks_north(grid, *x - 1, *y - 1)) {
        swap(grid, *x, *y, *x, *y - 1);
        (*y)--;
      }
    }
  } else if (instruction == '>') {
    char next = grid[*y][*x + 1];
    if (next == '.') {
      grid[*y][(*x)++] = '.';
      grid[*y][*x] = '@';
    } else if (next == '[') {
      size_t nx = *x + 1;
      while (grid[*y][nx] == '[' || grid[*y][nx] == ']') {
        nx++;
      }
      if (grid[*y][nx] == '.') {
        for (size_t i = nx; i > *x; i--) {
          grid[*y][i] = grid[*y][i - 1];
        }
        grid[*y][(*x)++] = '.';
      }
    }
  } else if (instruction == 'v') {
    char next = grid[*y + 1][*x];
    if (next == '.') {
      grid[(*y)++][*x] = '.';
      grid[*y][*x] = '@';
    } else if (next == '[') {
      if (move_blocks_south(grid, *x, *y + 1)) {
        swap(grid, *x, *y, *x, *y + 1);
        (*y)++;
      }
    } else if (next == ']') {
      if (move_blocks_south(grid, *x - 1, *y + 1)) {
        swap(grid, *x, *y, *x, *y + 1);
        (*y)++;
      }
    }
  } else if (instruction == '<') {
    char next = grid[*y][*x - 1];
    if (next == '.') {
      grid[*y][(*x)--] = '.';
      grid[*y][*x] = '@';
    } else if (next == ']') {
      size_t nx = *x - 1;
      while (grid[*y][nx] == '[' || grid[*y][nx] == ']') {
        nx--;
      }
      if (grid[*y][nx] == '.') {
        for (size_t i = nx; i < *x; i++) {
          grid[*y][i] = grid[*y][i + 1];
        }
        grid[*y][(*x)--] = '.';
      }
    }
  }
}

i64 part2(StringArray *lines) {
  StringArray *warehouse = string_array_new();
  StringArray *instructions = string_array_new();

  string_array_foreach (char *line, lines) {
    size_t line_len = strlen(line);
    if (line_len > 0) {
      if (line[0] == '#') {
        char buffer[1000] = {};
        size_t b = 0;
        for (size_t i = 0; i < line_len; i++) {
          if (line[i] == '#') {
            buffer[b++] = '#';
            buffer[b++] = '#';
          } else if (line[i] == 'O') {
            buffer[b++] = '[';
            buffer[b++] = ']';
          } else if (line[i] == '.') {
            buffer[b++] = '.';
            buffer[b++] = '.';
          } else if (line[i] == '@') {
            buffer[b++] = '@';
            buffer[b++] = '.';
          }
        }
        string_array_push(warehouse, buffer);
      } else {
        string_array_push(instructions, line);
      }
    }
  }
  debugf(string_array_print_raw, warehouse);
  debugf(string_array_print, instructions);

  size_t cols = strlen(warehouse->items[0]);
  size_t rows = warehouse->size;

  size_t x = 1, y = 1;
  for (size_t j = 0; j < rows; j++) {
    for (size_t i = 0; i < cols; i++) {
      if (warehouse->items[j][i] == '@') {
        x = i;
        y = j;
        debug("robot found at (%zu, %zu)\n", x, y);
      }
    }
  }

  string_array_enumerate (char *line, j, instructions) {
    size_t line_len = strlen(line);
    for (size_t i = 0; i < line_len; i++) {
      trace("next instruction: %c (%zu/%zu, %zu/%zu)\n", line[i], i + 1, line_len, j + 1, instructions->size);
#ifdef TRACE
      printf("press enter...\n");
      getchar();
#endif
      move_part2(warehouse->items, &x, &y, line[i]);
      tracef(string_array_print_raw, warehouse);
    }
  }
  debugf(string_array_print_raw, warehouse);

  i64 result = 0;
  for (size_t j = 0; j < rows; j++) {
    for (size_t i = 0; i < cols; i++) {
      if (warehouse->items[j][i] == '[') {
        result += 100 * j + i;
      }
    }
  }

  string_array_free(instructions);
  string_array_free(warehouse);

  return result;
}

int main(void) {
  test_case(day15, part1, sample1, 2028);
  test_case(day15, part1, sample2, 10092);
  test_case(day15, part1, data, 1487337);
  test_case(day15, part2, sample2, 9021);
  test_case(day15, part2, data, 1521952);

  return 0;
}
