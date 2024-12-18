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

i64 bfs(char **grid, size_t cols, size_t rows, size_t sx, size_t sy, size_t ex, size_t ey) {
  Array *q = array_new(Q);
  array_push(q, &(Q) {
    .position = {
      .x = sx,
      .y = sy
    },
    .steps = 0
  });

  bool **seen = malloc(sizeof(*seen) * rows);
  for (size_t y = 0; y < rows; y++) {
    seen[y] = calloc(cols, sizeof(**seen));
  }
  
  i64 result = -1;

  while (q->size > 0) {
    Q *current = array_shift(q);
    size_t x = current->position.x;
    size_t y = current->position.y;
    i64 s = current->steps;

    if (seen[y][x]) {
      free(current);
      continue;
    }
    seen[y][x] = true;

    if (x == ex && y == ey) {
      result = s;
      free(current);
      break;
    }

    if (y > 0 && grid[y - 1][x] != '#' && !seen[y - 1][x]) {
      array_push(q, &(Q) { .position = { .x = x, .y = y - 1 }, .steps = s + 1 });
    }
    if (x < cols - 1 && grid[y][x + 1] != '#' && !seen[y][x + 1]) {
      array_push(q, &(Q) { .position = { .x = x + 1, .y = y }, .steps = s + 1 });
    }
    if (y < rows - 1 && grid[y + 1][x] != '#' && !seen[y + 1][x]) {
      array_push(q, &(Q) { .position = { .x = x, .y = y + 1}, .steps = s + 1 });
    }
    if (x > 0 && grid[y][x - 1] != '#' && !seen[y][x - 1]) {
      array_push(q, &(Q) { .position = { .x = x - 1, .y = y }, .steps = s + 1 });
    }

    free(current);
  }

  array_free(q);
  for (size_t y = 0; y < rows; y++) {
    free(seen[y]);
  }
  free(seen);

  return result;
}

i64 part1(StringArray *lines, size_t range) {
  i64 result = 0;

  size_t cols = 0, rows = 0;
  Array *blocks = array_new(Vec2); 

  string_array_foreach (char *line, lines) {
    size_t x, y;
    if (sscanf(line, "%zu,%zu", &x, &y) == 2) {
      trace("(%zu, %zu), %zux%zu\n", x, y, cols, rows);

      if (x > cols) cols = x;
      if (y > rows) rows = y;

      array_push(blocks, &(Vec2) { .x = x, .y = y });
    }
  }

  cols++;
  rows++;
  debug("grid is %zux%zu\n", cols, rows);

  char **grid = malloc(sizeof(*grid) * rows);
  for (size_t y = 0; y < rows; y++) {
    grid[y] = calloc(cols + 1, sizeof(**grid));
    for (size_t x = 0; x < cols; x++) {
      grid[y][x] = '.';
    }
  }

  for (size_t r = 0; r < range; r++) {
    Vec2 *b = array_get(blocks, r);
    grid[b->y][b->x] = '#';
  }

#ifdef DEBUG
  for (size_t y = 0; y < rows; y++) {
    printf("%s\n", grid[y]);
  }
#endif

  result = bfs(grid, cols, rows, 0, 0, cols - 1, rows - 1);

  array_free(blocks);
  for (size_t y = 0; y < rows; y++) {
    free(grid[y]);
  }
  free(grid);

  return result;
}

i64 attempt_bfs(Array *blocks, size_t ei, size_t cols, size_t rows) {
  char **grid = malloc(sizeof(*grid) * rows);
  for (size_t y = 0; y < rows; y++) {
    grid[y] = calloc(cols + 1, sizeof(*grid));
    for (size_t x = 0; x < cols; x++) {
      grid[y][x] = '.';
    }
  }

  for (size_t i = 0; i < ei; i++) {
    Vec2 *b = array_get(blocks, i);
    grid[b->y][b->x] = '#';
  }

  i64 steps = bfs(grid, cols, rows, 0, 0, cols - 1, rows - 1);

  for (size_t y = 0; y < rows; y++) {
    free(grid[y]);
  }
  free(grid);

  return steps;
}

i64 part2(StringArray *lines, size_t ex, size_t ey) {
  size_t cols = 0, rows = 0;
  Array *blocks = array_new(Vec2); 

  string_array_foreach (char *line, lines) {
    size_t x, y;
    if (sscanf(line, "%zu,%zu", &x, &y) == 2) {
      trace("(%zu, %zu), %zux%zu\n", x, y, cols, rows);

      if (x > cols) cols = x;
      if (y > rows) rows = y;

      array_push(blocks, &(Vec2) { .x = x, .y = y });
    }
  }

  cols++;
  rows++;
  debug("grid is %zux%zu\n", cols, rows);

  size_t min = 0;
  size_t max = blocks->size;
  while (min != max) {
    size_t i = min == max - 1 ? max : min + (max - min) / 2;
    debug("trying %zu (%zu, %zu)... ", i, min, max);
    i64 steps = attempt_bfs(blocks, i, cols, rows);
    if (steps == -1) {
      max = i - 1;
      debug("nope, next (%zu, %zu)\n", min, max);
    } else {
      min = i;
      debug("ok, next (%zu, %zu)\n", min, max);
    }
  }

  Vec2 *result = array_get(blocks, min);
  assert(result != nullptr);
  assert(result->x == ex && result->y == ey);

  array_free(blocks);

  return 0;
}

int main(void) {
  test_case(day18, part1, sample, 22, 12);
  test_case(day18, part1, data, 308, 1024);
  test_case(day18, part2, sample, 0, 6, 1);
  test_case(day18, part2, data, 0, 46, 28);

  return 0;
}
