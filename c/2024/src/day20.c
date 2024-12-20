#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

typedef struct {
  size_t x;
  size_t y;
} Vec2;

Vec2 directions[4] = {
  {  0, -1 }, // North
  {  1,  0 }, // East
  {  0,  1 }, // South
  { -1,  0 }, // West
};

typedef struct {
  Vec2 position;
  i64 steps;
} Q;

void q_array_print(Array *array) {
  printf("Array{ capacity = %zu, size = %zu, stride = %zu, data = [ ", array->capacity, array->size, array->stride);
  for (size_t i = 0; i < array->size; i++) {
    Q *q = array_get(array, i);
    if (i == array->size - 1) {
      printf("{ .p = (%zu, %zu), .s = %lld }, ", q->position.x, q->position.y, q->steps);
    } else {
      printf("{ .p = (%zu, %zu), .s = %lld }", q->position.x, q->position.y, q->steps);
    }
  }
  printf(" ] }\n");
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void flood_fill(char **grid, i64 **steps, size_t cols, size_t rows, size_t sx, size_t sy) {
  Array *q = array_new(Q);
  array_push(q, &(Q) {
    .position = {
      .x = sx,
      .y = sy
    },
    .steps = 0
  });

  while (q->size > 0) {
    tracef(q_array_print, q);

    Q *current = array_shift(q);
    size_t x = current->position.x;
    size_t y = current->position.y;
    i64 s = current->steps;

    if (grid[y][x] == 'E') {
      steps[y][x] = s;
      free(current);
      goto defer;
    }
    
    if (steps[y][x] < s) {
      free(current);
      continue;
    }
    steps[y][x] = s;

    for (i64 d = 0; d < 4; d++) {
      size_t dx = x + directions[d].x;
      size_t dy = y + directions[d].y;

      if (grid[dy][dx] == '#' || steps[dy][dx] < s) {
        continue;
      }

      array_push(q, &(Q) {
        .position = {
          .x = dx,
          .y = dy,
        },
        .steps = s + 1
      });
    }
    free(current);
  }

  defer:
#ifdef DEBUG
  for (size_t y = 0; y < rows; y++) {
    for (size_t x = 0; x < cols; x++) {
      if (grid[y][x] == '#') {
        printf(" #####");
      } else {
        printf("%6lld", steps[y][x]);
      }
    }
    printf("\n");
  }
#endif

  array_free(q);
}

i64 cheats_part1(char **grid, i64 **steps, size_t cols, size_t rows, size_t sx, size_t sy, i64 gained) {
  size_t x = sx, y = sy;
  i64 s = 0;

  i64 result = 0;
  while (grid[y][x] != 'E') {
    assert(s == steps[y][x]);
    debug("(%zu, %zu, %lld)\n", x, y, s);

    if (y > 1 && grid[y - 1][x] == '#' && grid[y - 2][x] != '#') {
      i64 g = steps[y - 2][x] - s - 2;
      debug("peak up at (%zu, %zu) for %lld gain\n", x, y - 2, g);
      if (g >= gained) result++;
    }
    if (x < cols - 2 && grid[y][x + 1] == '#' && grid[y][x + 2] != '#') {
      i64 g = steps[y][x + 2] - s - 2;
      debug("peak right at (%zu, %zu) for %lld gain\n", x + 2, y, g);
      if (g >= gained) result++;
    }
    if (y < rows - 2 && grid[y + 1][x] == '#' && grid[y + 2][x] != '#') {
      i64 g = steps[y + 2][x] - s - 2;
      debug("peak down at (%zu, %zu) for %lld gain\n", x, y + 2, g);
      if (g >= gained) result++;
    }
    if (x > 1 && grid[y][x - 1] == '#' && grid[y][x - 2] != '#') {
      i64 g = steps[y][x - 2] - s - 2;
      debug("peak left at (%zu, %zu) for %lld gain\n", x - 2, y, g);
      if (g >= gained) result++;
    }

    for (i64 d = 0; d < 4; d++) {
      size_t dx = x + directions[d].x;
      size_t dy = y + directions[d].y;

      if (grid[dy][dx] == '#') {
        continue;
      }

      i64 ds = s + 1;
      if (steps[dy][dx] == ds) {
        x = dx;
        y = dy;
        s = ds;
        break;
      }
    }
  }

  return result;
}

i64 part1(StringArray *lines, i64 gained) {
  size_t cols = strlen(string_array_get(lines, 0));
  size_t rows = lines->size;
  char **grid = lines->items;

  tracef(string_array_print_raw, lines);
  trace("\n");

  i64 **steps = malloc(sizeof(*steps) * rows);
  for (size_t y = 0; y < rows; y++) {
    steps[y] = calloc(cols, sizeof(**steps));
    for (size_t x = 0; x < cols; x++) {
      steps[y][x] = INT64_MAX;
    }
  }

  size_t sx = 0, sy = 0;
  for (size_t y = 0; y < rows; y++) {
    for (size_t x = 0; x < cols; x++) {
      if (grid[y][x] == 'S') {
        sx = x;
        sy = y;
      }
    }
  }
  assert(sx != 0 && sy != 0);

  flood_fill(grid, steps, cols, rows, sx, sy);
  i64 result = cheats_part1(grid, steps, cols, rows, sx, sy, gained);

  free(steps);

  return result;
}

i64 peak(char **grid, i64 **steps, size_t cols, size_t rows, size_t sx, size_t sy, i64 s, i64 gained, i64 range) {
  i64 ssx = sx, ssy = sy, scols = cols, srows = rows;

  i64 result = 0;
  for (i64 j = ssy - range; j <= ssy + range; j++) {
    for (i64 i = ssx - range; i <= ssx + range; i++) {
      i64 x = ssx < i ? i - ssx : ssx - i;
      i64 y = ssy < j ? j - ssy : ssy - j;
      i64 d = x + y;
      if (0 <= i && i < scols && 0 <= j && j < srows && (i != ssx || j != ssy) && d <= range) {
        size_t ux = i, uy = j, ud = d;
        if (grid[uy][ux] != '#' && s < steps[uy][ux]) {
          i64 g = steps[uy][ux] - s - ud;
          if (g >= gained) {
            result++;
          }
        }
      }
    }
  }

  return result;
}

i64 cheats_part2(char **grid, i64 **steps, size_t cols, size_t rows, size_t sx, size_t sy, i64 gained, i64 range) {
  size_t x = sx, y = sy;
  i64 s = 0;

  bool **seen = malloc(sizeof(*seen) * rows);
  for (size_t y = 0; y < rows; y++) {
    seen[y] = calloc(cols, sizeof(**seen));
  }

  i64 result = 0;
  while (grid[y][x] != 'E') {
    debug("(%zu, %zu, %lld)\n", x, y, s);
    result += peak(grid, steps, cols, rows, x, y, s, gained, range);

    for (i64 d = 0; d < 4; d++) {
      size_t dx = x + directions[d].x;
      size_t dy = y + directions[d].y;

      if (grid[dy][dx] == '#') {
        continue;
      }

      i64 ds = s + 1;
      if (steps[dy][dx] == ds) {
        x = dx;
        y = dy;
        s = ds;
        break;
      }
    }
  }

  for (size_t y = 0; y < rows; y++) {
    free(seen[y]);
  }
  free(seen);

  return result;
}

i64 part2(StringArray *lines, i64 gained, i64 range) {
  size_t cols = strlen(string_array_get(lines, 0));
  size_t rows = lines->size;
  char **grid = lines->items;

  tracef(string_array_print_raw, lines);
  trace("\n");

  i64 **steps = malloc(sizeof(*steps) * rows);
  for (size_t y = 0; y < rows; y++) {
    steps[y] = calloc(cols, sizeof(**steps));
    for (size_t x = 0; x < cols; x++) {
      steps[y][x] = INT64_MAX;
    }
  }

  size_t sx = 0, sy = 0;
  for (size_t y = 0; y < rows; y++) {
    for (size_t x = 0; x < cols; x++) {
      if (grid[y][x] == 'S') {
        sx = x;
        sy = y;
      }
    }
  }
  assert(sx != 0 && sy != 0);

  flood_fill(grid, steps, cols, rows, sx, sy);
  i64 result = cheats_part2(grid, steps, cols, rows, sx, sy, gained, range);

  free(steps);

  return result;
}

int main(void) {
  //test_case(day20, part1, sample, 14, 2);
  //test_case(day20, part1, sample, 14, 4);
  //test_case(day20, part1, sample, 2, 6);
  //test_case(day20, part1, sample, 4, 8);
  //test_case(day20, part1, sample, 2, 10);
  //test_case(day20, part1, sample, 3, 12);
  //test_case(day20, part1, sample, 1, 20);
  //test_case(day20, part1, sample, 1, 36);
  //test_case(day20, part1, sample, 1, 38);
  //test_case(day20, part1, sample, 1, 40);
  //test_case(day20, part1, sample, 1, 64);
  test_case(day20, part1, data, 1454, 100);
  //test_case(day20, part2, sample, 14, 2, 2);
  //test_case(day20, part2, sample, 14, 4, 2);
  //test_case(day20, part2, sample, 2, 6, 2);
  //test_case(day20, part2, sample, 4, 8, 2);
  //test_case(day20, part2, sample, 2, 10, 2);
  //test_case(day20, part2, sample, 3, 12, 2);
  //test_case(day20, part2, sample, 1, 20, 2);
  //test_case(day20, part2, sample, 1, 36, 2);
  //test_case(day20, part2, sample, 1, 38, 2);
  //test_case(day20, part2, sample, 1, 40, 2);
  //test_case(day20, part2, sample, 1, 64, 2);
  //test_case(day20, part2, sample, 32, 50, 20);
  //test_case(day20, part2, sample, 31, 52, 20);
  //test_case(day20, part2, sample, 29, 54, 20);
  //test_case(day20, part2, sample, 39, 56, 20);
  //test_case(day20, part2, sample, 25, 58, 20);
  //test_case(day20, part2, sample, 23, 60, 20);
  //test_case(day20, part2, sample, 20, 62, 20);
  //test_case(day20, part2, sample, 19, 64, 20);
  //test_case(day20, part2, sample, 12, 66, 20);
  //test_case(day20, part2, sample, 14, 68, 20);
  //test_case(day20, part2, sample, 12, 70, 20);
  //test_case(day20, part2, sample, 22, 72, 20);
  //test_case(day20, part2, sample, 4, 74, 20);
  //test_case(day20, part2, sample, 3, 76, 20);
  test_case(day20, part2, data, 997879, 100, 20);

  return 0;
}
