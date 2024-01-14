#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "string.h"

bool **seen_new(size_t mx, size_t my) {
  bool **seen = malloc(sizeof(bool *) * my);
  for (size_t y = 0; y < my; y++) {
    seen[y] = calloc(mx, sizeof(bool));
    for (size_t x = 0; x < mx ; x++) {
      seen[y][x] = false;
    }
  }
  return seen;
}

void seen_free(bool **seen, size_t my) {
  for (size_t y = 0; y < my; y++) {
    free(seen[y]);
  }
  free(seen);
}

typedef struct {
  size_t x;
  size_t y;
} Vec2;

void dfs(char **trail, bool **seen, size_t mx, size_t my, size_t x, size_t y, int increment, int *max) {
  Array *vecs = array_new(Vec2);
  if (y > 0 && !seen[y - 1][x] && (trail[y - 1][x] == '.' || trail[y - 1][x] == '^')) {
    array_push(vecs, &(Vec2) { x, y - 1 });
  }
  if (x > 0 && !seen[y][x - 1] && (trail[y][x - 1] == '.' || trail[y][x - 1] == '<')) {
    array_push(vecs, &(Vec2) { x - 1, y });
  }
  if (y < my - 1 && !seen[y + 1][x] && (trail[y + 1][x] == '.' || trail[y + 1][x] == 'v')) {
    array_push(vecs, &(Vec2) { x, y + 1 });
  }
  if (x < mx - 1 && !seen[y][x + 1] && (trail[y][x + 1] == '.' || trail[y][x + 1] == '>')) {
    array_push(vecs, &(Vec2) { x + 1, y });
  }

  if (vecs->size == 0 && x == mx - 2 && y == mx - 1) {
    if (increment > *max) {
      debug("new max = %d\n", increment);
      *max = increment;
    }
  }

  for (size_t i = 0; i < vecs->size; i++) {
    Vec2 *u = array_get(vecs, i);

    seen[y][x] = true;
    dfs(trail, seen, mx, my, u->x, u->y, increment + 1, max);
    seen[y][x] = false;
  }

  array_free(vecs);
}

// GCC reports seen as uninitiliazed when not printing trail?
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
void dfs2(char **trail, bool **seen, size_t mx, size_t my, size_t x, size_t y, int increment, int *max) {
  if (x == mx - 2 && y == mx - 1) {
    if (increment > *max) {
      debug("new max = %d\n", increment);
      *max = increment;
    }
    return;
  }

  if (y > 0 && !seen[y - 1][x] && trail[y - 1][x] != '#') {
    seen[y][x] = true;
    dfs2(trail, seen, mx, my, x, y - 1, increment + 1, max);
    seen[y][x] = false;
  }
  if (x > 0 && !seen[y][x - 1] && trail[y][x - 1] != '#') {
    seen[y][x] = true;
    dfs2(trail, seen, mx, my, x - 1, y, increment + 1, max);
    seen[y][x] = false;
  }
  if (y < my - 1 && !seen[y + 1][x] && trail[y + 1][x] != '#') {
    seen[y][x] = true;
    dfs2(trail, seen, mx, my, x, y + 1, increment + 1, max);
    seen[y][x] = false;
  }
  if (x < mx - 1 && !seen[y][x + 1] && trail[y][x + 1] != '#') {
    seen[y][x] = true;
    dfs2(trail, seen, mx, my, x + 1, y, increment + 1, max);
    seen[y][x] = false;
  }
}

int part1(StringArray *lines) {
  debugf(string_array_print_raw, lines);

  size_t mx = strlen(lines->items[0]), my = lines->size;
  size_t sx = 1, sy = 0;

  int result = -1;
  bool **seen = seen_new(mx, my);
  dfs(lines->items, seen, mx, my, sx, sy, 0, &result);

  seen_free(seen, my);

  return result;
}

int part2(StringArray *lines) {
  debugf(string_array_print_raw, lines);

  size_t mx = strlen(lines->items[0]), my = lines->size;
  size_t sx = 1, sy = 0;

  int result = -1;
  bool **seen = seen_new(mx, my);
  dfs2(lines->items, seen, mx, my, sx, sy, 0, &result);

  seen_free(seen, my);

  return result;
}

int main(void) {
  test_case(day23, part1, sample, 94);
  test_case(day23, part1, data, 2154);
  test_case(day23, part2, sample, 154);
  test_case(day23, part2, data, 6654);

  return 0;
}
