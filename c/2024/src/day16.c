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
  i64 direction;
  i64 score;
} Q;

void q_array_print(Array *array) {
  printf("Array{ capacity = %zu, size = %zu, stride = %zu, data = [ ", array->capacity, array->size, array->stride);
  for (size_t i = 0; i < array->size; i++) {
    Q *q = array_get(array, i);
    if (i == array->size - 1) {
      printf("{ .p = (%zu, %zu), .d = %lld, .s = %lld }, ", q->position.x, q->position.y, q->direction, q->score);
    } else {
      printf("{ .p = (%zu, %zu), .d = %lld, .s = %lld }", q->position.x, q->position.y, q->direction, q->score);
    }
  }
  printf(" ] }\n");
}

void array_priority_push(Array *array, Q *value) {
  if (array->size == 0) {
    array_push(array, value);
  } else {
    size_t i = 0;
    while (i < array->size) {
      Q *other = array_get(array, i);
      if (other->score > value->score) {
        break;
      }
      i++;
    }
    array_insert(array, i, value);
  }
}

#define dim(x, y, d) ((x)+(y)*cols+(d)*cols*rows)

i64 dijkstra(char **grid, i64 *scores, bool *seen, size_t cols, size_t rows, size_t sx, size_t sy) {
  Array *q = array_new(Q);
  array_push(q, &(Q) {
    .position = {
      .x = sx,
      .y = sy
    },
    .direction = 1,
    .score = 0
  });

  i64 score = -1;

  while (q->size > 0) {
    debugf(q_array_print, q);

    Q *current = array_shift(q);
    size_t x = current->position.x;
    size_t y = current->position.y;
    i64 d = current->direction;
    i64 s = current->score;

    if (grid[y][x] == 'E') {
      score = s;
      free(current);
      goto defer;
    }

    if (seen[dim(x, y, d)]) {
      free(current);
      continue;
    }
    seen[dim(x, y, d)] = true;

    for (i64 dd = 0; dd < 4; dd++) {
      if ((dd + 2) % 4 == d) {
        continue;
      }

      size_t dx = (dd != d) ? x : x + directions[dd].x;
      size_t dy = (dd != d) ? y : y + directions[dd].y;

      if (grid[dy][dx] == '#') {
        continue;
      }

      i64 ds = s;
      if ((d + 1) % 4 == dd || (d == 0 ? 3 : d - 1) == dd) {
        ds += 1000;
      } else {
        ds++;
      }

      if (scores[dim(dx, dy, dd)] < ds) {
        continue;
      }
      scores[dim(dx, dy, dd)] = ds;

      array_priority_push(q, &(Q) {
        .position = {
          .x = dx,
          .y = dy,
        },
        .direction = dd,
        .score = ds
      });
    }
    free(current);
  }

  defer:
#ifdef DEBUG
  for (size_t y = 0; y < rows; y++) {
    for (size_t x = 0; x < cols; x++) {
      if (grid[y][x] == '#') printf(" #####");
      else if (grid[y][x] == 'E' || grid[y][x] == 'S') printf("%6c", grid[y][x]);
      else if (grid[y][x] == '.') {
        i64 s = INT64_MAX;
        for (i64 d = 0; d < 4; d++) {
          if (scores[dim(x, y, d)] < s) {
            s = scores[dim(x, y, d)];
          }
        }
        if (s < INT64_MAX) printf("%6lld", s);
        else printf(" .....");
      }
    }
    printf("\n");
  }
#endif

  array_free(q);

  return score;
}

i64 part1(StringArray *lines) {
  size_t cols = strlen(string_array_get(lines, 0));
  size_t rows = lines->size;
  char **grid = lines->items;

  tracef(string_array_print_raw, lines);
  trace("\n");

  i64 *scores = malloc(sizeof(*scores) * 4 * rows * cols);
  bool *seen = malloc(sizeof(*seen) * 4 * rows * cols);
  for (size_t d = 0; d < 4; d++) {
    for (size_t y = 0; y < rows; y++) {
      for (size_t x = 0; x < cols; x++) {
        scores[dim(x, y, d)] = INT64_MAX;
        seen[dim(x, y, d)] = false;
      }
    }
  }

  i64 result = dijkstra(grid, scores, seen, cols, rows, 1, rows - 2);

  free(scores);
  free(seen);

  return result;
}

i64 flood_fill(char **grid, i64 *scores, size_t cols, size_t rows, size_t sx, size_t sy) {
  Array *q = array_new(Q);
  array_push(q, &(Q) {
    .position = {
      .x = sx,
      .y = sy
    },
    .direction = 1,
    .score = 0
  });

  i64 score = INT64_MAX;
  while (q->size > 0) {
    Q *current = array_shift(q);
    size_t x = current->position.x;
    size_t y = current->position.y;
    i64 d = current->direction;
    i64 s = current->score;

    if (grid[y][x] == 'E' && s < score) {
      score = s;
    }

    if (scores[dim(x, y, d)] < s) {
      free(current);
      continue;
    }
    scores[dim(x, y, d)] = s;

    i64 d1 = (d + 1) % 4;
    if (s + 1000 < scores[dim(x, y, d1)]) {
      array_push(q, &(Q) { .position = { .x = x, .y = y }, .direction = d1, .score = s + 1000 });
    }

    size_t d2x = x + directions[d].x;
    size_t d2y = y + directions[d].y;
    if (grid[d2y][d2x] != '#' && s + 1 < scores[dim(d2x, d2y, d)]) {
      array_push(q, &(Q) { .position = { .x = d2x, .y = d2y }, .direction = d, .score = s + 1 });
    }

    i64 d3 = (d + 3) % 4;
    if (s + 1000 < scores[dim(x, y, d3)]) {
      array_push(q, &(Q) { .position = { .x = x, .y = y }, .direction = d3, .score = s + 1000 });
    }

    free(current);
  }

#ifdef DEBUG
  for (size_t y = 0; y < rows; y++) {
    for (size_t x = 0; x < cols; x++) {
      if (grid[y][x] == '#') printf(" #####");
      else if (grid[y][x] == 'E' || grid[y][x] == 'S') printf("%6c", grid[y][x]);
      else if (grid[y][x] == '.') {
        i64 s = INT64_MAX;
        for (i64 d = 0; d < 4; d++) {
          if (scores[dim(x, y, d)] < s) {
            s = scores[dim(x, y, d)];
          }
        }
        if (s < INT64_MAX) printf("%6lld", s);
        else printf(" .....");
      }
    }
    printf("\n");
  }
#endif

  array_free(q);

  return score;
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
i64 reverse(char **grid, i64 *scores, size_t cols, size_t rows, size_t sx, size_t sy, i64 best_score) {
  Array *q = array_new(Q);
  for (i64 d = 0; d < 4; d++) {
    if (scores[dim(sx, sy, d)] == best_score) {
      array_push(q, &(Q) {
        .position = {
          .x = sx,
          .y = sy
        },
        .direction = d,
        .score = best_score
      });
    }
  }

  bool **reversed = malloc(sizeof(*reversed) * rows);
  for (size_t y = 0; y < rows; y++) {
    reversed[y] = calloc(cols, sizeof(**reversed));
  }

  i64 result = 0;
  while (q->size > 0) {
    tracef(q_array_print, q);

    Q *current = array_shift(q);
    size_t x = current->position.x;
    size_t y = current->position.y;
    i64 d = current->direction;
    i64 s = current->score;

    if (!reversed[y][x]) {
      result++;
    }
    reversed[y][x] = true;

    i64 d1 = (d + 1) % 4;
    i64 s1 = scores[dim(x, y, d1)];
    if (s1 + 1000 == s) {
      trace("rotation 1: (x=%zu, y=%zu, d=%lld, s=%lld)\n", x, y, d1, s1);
      array_push(q, &(Q) { .position = { .x = x, .y = y }, .direction = d1, .score = s1 });
    }

    size_t d2x = x - directions[d].x;
    size_t d2y = y - directions[d].y;
    i64 s2 = scores[dim(d2x, d2y, d)];
    trace("opposite move: (x=%zu, y=%zu, d=%lld, s=%lld)\n", d2x, d2y, d, s2);
    if (s2 + 1 == s) {
      array_push(q, &(Q) { .position = { .x = d2x, .y = d2y }, .direction = d, .score = s2 });
    }

    i64 d3 = (d + 3) % 4;
    i64 s3 = scores[dim(x, y, d3)];
    if (s3 + 1000 == s) {
      trace("rotation 3: (x=%zu, y=%zu, d=%lld, s=%lld)\n", x, y, d3, s3);
      array_push(q, &(Q) { .position = { .x = x, .y = y }, .direction = d3, .score = s3 });
    }

    free(current);
  }

#ifdef DEBUG
  for (size_t y = 0; y < rows; y++) {
    for (size_t x = 0; x < cols; x++) {
      if (grid[y][x] == '#') printf("#");
      else if (reversed[y][x]) printf("\033[91;1mO\033[0m");
      else printf(".");
    }
    printf("\n");
  }
#endif

  array_free(q);
  for (size_t y = 0; y < rows; y++) {
    free(reversed[y]);
  }
  free(reversed);

  return result;
}

i64 part2(StringArray *lines, i64 dijkstra) {
  size_t cols = strlen(string_array_get(lines, 0));
  size_t rows = lines->size;
  char **grid = lines->items;

  tracef(string_array_print_raw, lines);
  trace("\n");

  i64 *scores = malloc(sizeof(*scores) * 4 * rows * cols);
  for (size_t d = 0; d < 4; d++) {
    for (size_t y = 0; y < rows; y++) {
      for (size_t x = 0; x < cols; x++) {
        scores[dim(x, y, d)] = INT64_MAX;
      }
    }
  }

  i64 score = flood_fill(grid, scores, cols, rows, 1, rows - 2);
  assert(score == dijkstra);

  i64 result = reverse(grid, scores, cols, rows, cols - 2, 1, score);

  free(scores);

  return result;
}

int main(void) {
  test_case(day16, part1, sample1, 7036);
  test_case(day16, part1, sample2, 11048);
  test_case(day16, part1, data, 72428);
  test_case(day16, part2, sample1, 45, 7036);
  test_case(day16, part2, sample2, 64, 11048);
  test_case(day16, part2, data, 456, 72428);

  return 0;
}
