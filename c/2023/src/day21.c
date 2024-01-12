#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "string.h"

constexpr size_t BUFFER_LENGTH = 1024;

typedef struct {
  int step;
  long long x;
  long long y;
} Coord;

typedef struct {
  long long ix;
  long long iy;
  StringArray *map;
} Space;

Coord find_start(char **map, long long mx, long long my) {
  for (long long y = 0; y < my; y++) {
    for (long long x = 0; x < mx; x++) {
      if (map[y][x] == 'S') {
        return ((Coord) { .step = 0, .x = x, .y = y });
      }
    }
  }
  return ((Coord) { .step = -1, .x = 0, .y = 0 });
}

StringArray *find_map(Array *infinity, long long ix, long long iy) {
  for (size_t i = 0; i < infinity->size; i++) {
    Space *s = array_get(infinity, i);
    if (s->ix == ix && s->iy == iy) {
      return s->map;
    }
  }
  return nullptr;
}

StringArray *find_map_or_dup(Array *infinity, StringArray *original, long long ix, long long iy) {
  StringArray *map = find_map(infinity, ix, iy);
  if (map == nullptr) {
    //printf("new map for ix = %lld, iy = %lld\n", ix, iy);
    map = string_array_dup(original);
    array_push(infinity, ((Space) { .ix = ix, .iy = iy, .map = map }));
  }
  return map;
}

void infinity_print_raw(Array *infinity, StringArray *map, long long six, long long eix, long long siy, long long eiy, long long my) {
  for (long long iy = siy; iy <= eiy; iy++) {
    for (long long y = 0; y < my; y++) {
      for (long long ix = six; ix <= eix; ix++) {
        StringArray *imap = find_map_or_dup(infinity, map, ix, iy);
        printf("%s", imap->items[y]);
      }
      printf("\n");
    }
  }
}

void array_priority_push(Array *array, Coord c) {
  if (array->size == 0) {
    array_push(array, c);
  } else {
    size_t i = 0;
    Coord *o;
    do {
      o = array_get(array, i++);
    } while (o->step < c.step && i <= array->size);
    array_insert(array, i - 1, c);
  }
}

long long part1(char *filename, int steps) {
  FILE *fp = fopen(filename, "r");
  if (fp == nullptr) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  StringArray *map = string_array_new();

  char buffer[BUFFER_LENGTH] = {};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';
    string_array_push(map, buffer);
  }
  fclose(fp);

  string_array_print_raw(map);

  long long mx = strlen(map->items[0]);
  long long my = map->size;

  Coord start = find_start(map->items, mx, my);
  printf("S = %lld %lld\n", start.x, start.y);

  int step = 0;
  Array *q = array_new(Coord);
  array_push(q, start);

  while (42) {
    Coord *peek = array_first(q);
    if (peek->step == step + 1) {
      step++;
      if (step == steps) {
        break;
      }
    }

    Coord *c = array_pop(q);
    map->items[c->y][c->x] = '.';
    if (c->y > 0 && map->items[c->y - 1][c->x] == '.') {
      map->items[c->y - 1][c->x] = 'O';
      array_priority_push(q, ((Coord) { .step = c->step + 1, .x = c->x, .y = c->y - 1 }));
    }
    if (c->x > 0 && map->items[c->y][c->x - 1] == '.') {
      map->items[c->y][c->x - 1] = 'O';
      array_priority_push(q, ((Coord) { .step = c->step + 1, .x = c->x - 1, .y = c->y }));
    }
    if (c->y < my - 1 && map->items[c->y + 1][c->x] == '.') {
      map->items[c->y + 1][c->x] = 'O';
      array_priority_push(q, ((Coord) { .step = c->step + 1, .x = c->x, .y = c->y + 1 }));
    }
    if (c->x < mx - 1 && map->items[c->y][c->x + 1] == '.') {
      map->items[c->y][c->x + 1] = 'O';
      array_priority_push(q, ((Coord) { .step = c->step + 1, .x = c->x + 1, .y = c->y }));
    }
    free(c);
    //string_array_print_raw(map);
  }

  long long sum = q->size;
  printf("sum = %lld\n", sum);

  array_free(q);
  string_array_free(map);

  return sum;
}

long long part2(char *filename, int steps) {
  FILE *fp = fopen(filename, "r");
  if (fp == nullptr) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  StringArray *map = string_array_new();

  char buffer[BUFFER_LENGTH] = {};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';
    string_array_push(map, buffer);
  }
  fclose(fp);

  //string_array_print_raw(map);

  Array *infinity = array_new(Space);

  long long mx = strlen(map->items[0]);
  long long my = map->size;

  Coord start = find_start(map->items, mx, my);
  printf("S = %lld %lld\n", start.x, start.y);
  map->items[start.y][start.x] = '.';

  int step = 0;
  Array *q = array_new(Coord);
  array_push(q, start);

  Array *n = array_new(long long);

  while (step <= steps) {
    Coord *peek = array_first(q);
    if (peek->step == step+1) {
      step++;
      //infinity_print_raw(infinity, map, -1, 1, -1, 1, my);
      if (step % mx == mx / 2) { // step % 131 == 65
        printf("step %d: %zu\n", step, q->size);
        array_push(n, q->size);
      }
      if (step == steps) {
        break;
      }
    }

    Coord *c = array_pop(q);
    long long ix = (c->x < 0) ? -((-c->x - 1 + mx) / mx) : c->x / mx;
    long long iy = (c->y < 0) ? -((-c->y - 1 + my) / my) : c->y / my;
    StringArray *imap = find_map_or_dup(infinity, map, ix, iy);

    long long bx = (c->x % mx + mx) % mx;
    long long by = (c->y % my + my) % my;
    long long x = bx, y = by;
    //printf("(%lld %lld) (%lld %lld) (%lld %lld) (%lld %lld)\n", c->x, c->y, bx, by, mx, my, ix, iy);
    imap->items[y][x] = '.';

    StringArray *umap = imap;
    if (by - 1 < 0) {
      y = my - 1;
      umap = find_map_or_dup(infinity, map, ix, iy - 1);
    } else {
      y = by - 1;
    }
    if (umap->items[y][x] == '.') {
      umap->items[y][x] = 'O';
      array_priority_push(q, ((Coord) { .step = c->step + 1, .x = c->x, .y = c->y - 1 }));
    }
    y = by;

    StringArray *lmap = imap;
    if (bx - 1 < 0) {
      x = mx - 1;
      lmap = find_map_or_dup(infinity, map, ix - 1, iy);
    } else {
      x = bx - 1;
    }
    if (lmap->items[y][x] == '.') {
      lmap->items[y][x] = 'O';
      array_priority_push(q, ((Coord) { .step = c->step + 1, .x = c->x - 1, .y = c->y }));
    }
    x = bx;

    StringArray *bmap = imap;
    if (y + 1 >= my) {
      y = 0;
      bmap = find_map_or_dup(infinity, map, ix, iy + 1);
    } else {
      y = by + 1;
    }
    if (bmap->items[y][x] == '.') {
      bmap->items[y][x] = 'O';
      array_priority_push(q, ((Coord) { .step = c->step + 1, .x = c->x, .y = c->y + 1 }));
    }
    y = by;

    StringArray *rmap = imap;
    if (x + 1 >= mx) {
      x = 0;
      rmap = find_map_or_dup(infinity, map, ix + 1, iy);
    } else {
      x = bx + 1;
    }
    if (rmap->items[y][x] == '.') {
      rmap->items[y][x] = 'O';
      array_priority_push(q, ((Coord) { .step = c->step + 1, .x = c->x + 1, .y = c->y }));
    }
    free(c);
  }

  // steps <= 131*3+65
  // step % 131 == 65
  // 3606 32258 89460 175212
  // ^ 28652 57202 85752 
  //   ^ 28550 28550 
  //     ^    0

  // x = (26501365 - 65) / 131 = 202300
  // -> 3606 + 28652 * x + 28550 * x * (x - 1) / 2
  //  = 3606 + 28652 * 202300 + 28550 * 202300 * 202299 / 2
  //  = 584211423220706

  long long sum = q->size; 
  printf("sum = %lld\n", sum);

  array_free(q);
  for (size_t i = 0; i < infinity->size; i++) {
    Space *s = array_get(infinity, i);
    string_array_free(s->map);
  }
  array_free(infinity);
  string_array_free(map);

  return sum;
}

int main(void) {
  assert(part1("../../inputs/2023/day21/sample", 6) == 16);
  assert(part1("../../inputs/2023/day21/data", 64) == 3503);
  assert(part2("../../inputs/2023/day21/sample", 6) == 16);
  assert(part2("../../inputs/2023/day21/sample", 10) == 50);
  assert(part2("../../inputs/2023/day21/sample", 50) == 1594);
  assert(part2("../../inputs/2023/day21/sample", 100) == 6536);
  assert(part2("../../inputs/2023/day21/sample", 500) == 167004);
  assert(part2("../../inputs/2023/day21/sample", 1000) == 668697);
  assert(part2("../../inputs/2023/day21/sample", 5000) == 16733044);
  //assert(part2("../../inputs/2023/day21/data", 26501365) == 584211423220706);

  return 0;
}
