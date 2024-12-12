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

void vec2_array_print(Array *array) {
  printf("Array{ capacity = %zu, size = %zu, stride = %zu, data = [ ", array->capacity, array->size, array->stride);
  for (size_t i = 0; i < array->size; i++) {
    Vec2 *v = array_get(array, i);
    if (i == array->size - 1) {
      printf("(%zu, %zu)", v->x, v->y);
    } else {
      printf("(%zu, %zu), ", v->x, v->y);
    }
  }
  printf(" ] }\n");
}

i64 region_cost_part1(char **grid, bool **seen, size_t cols, size_t rows, size_t i, size_t j) {
  i64 area = 0;
  i64 perimeter = 0;

  char plot = grid[j][i];

  Array *q = array_new(Vec2);
  array_push(q, &(Vec2) { .x = i, .y = j });

  while (q->size > 0) {
    tracef(vec2_array_print, q);

    Vec2 *v = array_pop(q);
    if (seen[v->y][v->x]) {
      free(v);
      continue;
    }
    seen[v->y][v->x] = true;

    i64 p = 4;
    if (v->y > 0 && grid[v->y - 1][v->x] == plot) {
      p--;
      if (!seen[v->y - 1][v->x]) {
        array_push(q, &(Vec2) { .x = v->x, .y = v->y - 1 });
      }
    }
    if (v->x < cols - 1 && grid[v->y][v->x + 1] == plot) {
      p--;
      if (!seen[v->y][v->x + 1]) {
        array_push(q, &(Vec2) { .x = v->x + 1, .y = v->y });
      }
    }
    if (v->y < rows - 1 && grid[v->y + 1][v->x] == plot) {
      p--;
      if (!seen[v->y + 1][v->x]) {
        array_push(q, &(Vec2) { .x = v->x, .y = v->y + 1 });
      }
    }
    if (v->x > 0 && grid[v->y][v->x - 1] == plot) {
      p--;
      if (!seen[v->y][v->x - 1]) {
        array_push(q, &(Vec2) { .x = v->x - 1, .y = v->y });
      }
    }

    area++;
    perimeter += p;

    free(v);
  }

  i64 result = area * perimeter;
  debug("region %c: %lld * %lld = %lld\n", plot, area, perimeter, result);

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
      trace("inspecting %c (%zu, %zu)\n", grid[j][i], i, j);
      if (!seen[j][i]) {
        debug("not seen %c (%zu, %zu)\n", grid[j][i], i, j);
        result += region_cost_part1(grid, seen, cols, rows, i, j);
      }
    }
  }

  for (size_t j = 0; j < rows; j++) {
    free(seen[j]);
  }
  free(seen);

  return result;
}

i64 region_cost_part2(char **grid, bool **seen, size_t cols, size_t rows, size_t i, size_t j) {
  i64 area = 0;
  i64 sides = 0;

  char plot = grid[j][i];

  Array *q = array_new(Vec2);
  array_push(q, &(Vec2) { .x = i, .y = j });

  while (q->size > 0) {
    tracef(vec2_array_print, q);

    Vec2 *v = array_pop(q);
    if (seen[v->y][v->x]) {
      free(v);
      continue;
    }
    seen[v->y][v->x] = true;

    area++;
    if (
      (v->y == 0 && v->x == cols - 1) ||
      (
        (v->y == 0 || grid[v->y - 1][v->x] != plot) &&
        (v->x == cols - 1 || grid[v->y][v->x + 1] != plot)
      )
    ) {
      trace("corner UR on plot (%zu, %zu)\n", v->x, v->y);
      sides++;
    }
    if (
      (v->y == rows - 1 && v->x == cols - 1) ||
      (
        (v->y == rows - 1 || grid[v->y + 1][v->x] != plot) &&
        (v->x == cols - 1 || grid[v->y][v->x + 1] != plot)
      )
    ) {
      trace("corner DR on plot (%zu, %zu)\n", v->x, v->y);
      sides++;
    }
    if (
      (v->y == rows - 1 && v->x == 0) ||
      (
        (v->y == rows - 1 || grid[v->y + 1][v->x] != plot) &&
        (v->x == 0 || grid[v->y][v->x - 1] != plot)
      )
    ) {
      trace("corner DL on plot (%zu, %zu)\n", v->x, v->y);
      sides++;
    }
    if (
      (v->y == 0 && v->x == 0) ||
      (
        (v->y == 0 || grid[v->y - 1][v->x] != plot) &&
        (v->x == 0 || grid[v->y][v->x - 1] != plot)
      )
    ) {
      trace("corner UL on plot (%zu, %zu)\n", v->x, v->y);
      sides++;
    }

    if (
      v->y > 0 && grid[v->y - 1][v->x] == plot &&
      v->x < cols - 1 && grid[v->y][v->x + 1] == plot &&
      grid[v->y - 1][v->x + 1] != plot
    ) {
      trace("inner corner UR on plot (%zu, %zu)\n", v->x, v->y);
      sides++;
    }
    if (
      v->x < cols - 1 && grid[v->y][v->x + 1] == plot &&
      v->y < rows - 1 &&  grid[v->y + 1][v->x] == plot &&
      grid[v->y + 1][v->x + 1] != plot
    ) {
      trace("inner corner DR on plot (%zu, %zu)\n", v->x, v->y);
      sides++;
    }
    if (
      v->y < rows - 1 && grid[v->y + 1][v->x] == plot &&
      v->x > 0 &&  grid[v->y][v->x - 1] == plot &&
      grid[v->y + 1][v->x - 1] != plot
    ) {
      trace("inner corner DL on plot (%zu, %zu)\n", v->x, v->y);
      sides++;
    }
    if (
      v->y > 0 && grid[v->y - 1][v->x] == plot &&
      v->x > 0 &&  grid[v->y][v->x - 1] == plot &&
      grid[v->y - 1][v->x - 1] != plot
    ) {
      trace("inner corner UL on plot (%zu, %zu)\n", v->x, v->y);
      sides++;
    }

    if (v->y > 0 && grid[v->y - 1][v->x] == plot && !seen[v->y - 1][v->x]) {
      array_push(q, &(Vec2) { .x = v->x, .y = v->y - 1 });
    }
    if (v->x < cols - 1 && grid[v->y][v->x + 1] == plot && !seen[v->y][v->x + 1]) {
      array_push(q, &(Vec2) { .x = v->x + 1, .y = v->y });
    }
    if (v->y < rows - 1 && grid[v->y + 1][v->x] == plot && !seen[v->y + 1][v->x]) {
      array_push(q, &(Vec2) { .x = v->x, .y = v->y + 1 });
    }
    if (v->x > 0 && grid[v->y][v->x - 1] == plot && !seen[v->y][v->x - 1]) {
      array_push(q, &(Vec2) { .x = v->x - 1, .y = v->y });
    }

    free(v);
  }

  i64 result = area * sides;
  debug("region %c: %lld * %lld = %lld\n", plot, area, sides, result);

  return result;
}

i64 part2(StringArray *lines) {
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
      trace("inspecting %c (%zu, %zu)\n", grid[j][i], i, j);
      if (!seen[j][i]) {
        debug("not seen %c (%zu, %zu)\n", grid[j][i], i, j);
        result += region_cost_part2(grid, seen, cols, rows, i, j);
      }
    }
  }

  for (size_t j = 0; j < rows; j++) {
    free(seen[j]);
  }
  free(seen);

  return result;
}

int main(void) {
  test_case(day12, part1, sample1, 140);
  test_case(day12, part1, sample2, 772);
  test_case(day12, part1, sample3, 1930);
  test_case(day12, part1, data, 1450422);
  test_case(day12, part2, sample1, 80);
  test_case(day12, part2, sample2, 436);
  test_case(day12, part2, sample3, 1206);
  test_case(day12, part2, sample4, 236);
  test_case(day12, part2, sample5, 368);
  test_case(day12, part2, data, 906606);

  return 0;
}
