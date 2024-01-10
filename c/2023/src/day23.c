#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "string.h"

#define BUFFER_LENGTH 1024

bool **seen_new(size_t mx, size_t my) {
  bool **seen = malloc(sizeof(bool *) * my);
  for (size_t y = 0; y < my; y++) {
    seen[y] = malloc(sizeof(bool) * mx);
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
} Coord;

void dfs(char **trail, bool **seen, size_t mx, size_t my, size_t x, size_t y, int increment, int *max) {
  Array *coords = array_new(Coord);
  if (y > 0 && !seen[y - 1][x] && (trail[y - 1][x] == '.' || trail[y - 1][x] == '^')) {
    array_push(coords, ((Coord) { x, y - 1 }));
  }
  if (x > 0 && !seen[y][x - 1] && (trail[y][x - 1] == '.' || trail[y][x - 1] == '<')) {
    array_push(coords, ((Coord) { x - 1, y }));
  }
  if (y < my - 1 && !seen[y + 1][x] && (trail[y + 1][x] == '.' || trail[y + 1][x] == 'v')) {
    array_push(coords, ((Coord) { x, y + 1 }));
  }
  if (x < mx - 1 && !seen[y][x + 1] && (trail[y][x + 1] == '.' || trail[y][x + 1] == '>')) {
    array_push(coords, ((Coord) { x + 1, y }));
  }

  if (coords->size == 0 && x == mx - 2 && y == mx - 1) {
    if (increment > *max) {
      printf("new max = %d\n", increment);
      *max = increment;
    } 
  }

  for (size_t i = 0; i < coords->size; i++) {
    Coord *c = array_get(coords, i);

    seen[y][x] = true;
    dfs(trail, seen, mx, my, c->x, c->y, increment + 1, max);
    seen[y][x] = false;
  }

  array_free(coords);
}

void dfs2(char **trail, bool **seen, size_t mx, size_t my, size_t x, size_t y, int increment, int *max) {
  if (x == mx - 2 && y == mx - 1) {
    if (increment > *max) {
      printf("new max = %d\n", increment);
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

int part1(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  StringArray *trail = string_array_new();

  char buffer[BUFFER_LENGTH] = {0};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';
    
    string_array_push(trail, buffer);
  }
  fclose(fp);

  string_array_print_raw(trail);

  size_t mx = strlen(trail->items[0]), my = trail->size;
  size_t sx = 1, sy = 0;

  int max = -1;
  bool **seen = seen_new(mx, my);
  dfs(trail->items, seen, mx, my, sx, sy, 0, &max);
  printf("max = %d\n", max);

  seen_free(seen, my);
  string_array_free(trail);

  return max;
}

int part2(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  StringArray *trail = string_array_new();

  char buffer[BUFFER_LENGTH] = {0};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';
    
    string_array_push(trail, buffer);
  }
  fclose(fp);

  string_array_print_raw(trail);

  size_t mx = strlen(trail->items[0]), my = trail->size;
  size_t sx = 1, sy = 0;

  int max = -1;
  bool **seen = seen_new(mx, my);
  dfs2(trail->items, seen, mx, my, sx, sy, 0, &max);
  printf("max = %d\n", max);

  seen_free(seen, my);
  string_array_free(trail);

  return max;
}

int main(void) {
  assert(part1("../../inputs/2023/day23/sample") == 94);
  assert(part1("../../inputs/2023/day23/data") == 2154);
  assert(part2("../../inputs/2023/day23/sample") == 154);
  assert(part2("../../inputs/2023/day23/data") == 6654);

  return 0;
}
