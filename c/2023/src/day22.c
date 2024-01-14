#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "string.h"

typedef struct {
  int x;
  int y;
  int z;
} Vec3;

typedef struct {
  Vec3 start;
  Vec3 end;
} Brick;

void brick_array_print(Array *bricks) {
  for (size_t i = 0; i < bricks->size; i++) {
    Brick *brick = array_get(bricks, i);
    printf("{ %d %d %d, %d %d %d }, ", brick->start.x, brick->start.y, brick->start.z, brick->end.x, brick->end.y, brick->end.z );
  }
  printf("\n");
}

bool collides(Brick *a, Brick *b, int z) {
  bool xcol = min(a->start.x, a->end.x) <= max(b->start.x, b->end.x) &&
              max(a->start.x, a->end.x) >= min(b->start.x, b->end.x);
  bool ycol = min(a->start.y, a->end.y) <= max(b->start.y, b->end.y) &&
              max(a->start.y, a->end.y) >= min(b->start.y, b->end.y);
  bool zcol = min(a->start.z, a->end.z) + z <= max(b->start.z, b->end.z) &&
              max(a->start.z, a->end.z) + z >= min(b->start.z, b->end.z);
  return xcol && ycol && zcol;
}

bool fixed(Array *bricks) {
  for (size_t i = 0; i < bricks->size; i++) {
    Brick *bi = array_get(bricks, i);
    if (bi->start.z == 1 || bi->end.z == 1) continue;
    bool c = true;
    for (size_t j = 0; j < bricks->size; j++) {
      if (i == j) continue;
      Brick *bj = array_get(bricks, j);
      c = c && !collides(bi, bj, -1);
      /*if (c == false) {
        printf(
          "{ %d %d %d, %d %d %d } with { %d %d %d, %d %d %d }\n",
          bi->start.x, bi->start.y, bi->start.z, bi->end.x, bi->end.y, bi->end.z,
          bj->start.x, bj->start.y, bj->start.z, bj->end.x, bj->end.y, bj->end.z
        );
      }*/
    }
    if (c) return false;
  }
  return true;
}

int fall(Array *bricks) {
  int f = 0;
  for (size_t i = 0; i < bricks->size; i++) {
    Brick *a = array_get(bricks, i);
    int minz = min(a->start.z, a->end.z);
    if (minz == 1) {
      continue;
    }
    int max_fall_z = minz - 1;
    bool stop = false;
    bool has_fallen = false;
    for (int fz = 1; fz <= max_fall_z && !stop; fz++) {
      for (size_t j = 0; j < bricks->size && !stop; j++) {
        if (i == j) continue;
        Brick *b = array_get(bricks, j);
        stop = stop || collides(a, b, -1);
      }
      if (!stop) {
        has_fallen = true;
        a->start.z--;
        a->end.z--;
      }
    }
    if (has_fallen) {
      f++;
    }
  }
  return f;
}

int false_sum(int *ints, size_t size) {
  int sum = 0;
  for (size_t i = 0; i < size; i++) {
    sum += ints[i] == 0 ? 0 : 1;
  }
  return sum;
}

int *fall2(Array *bricks) {
  int *fallen = malloc(sizeof(int) * bricks->size);
  memset(fallen, 0, sizeof(int) * bricks->size);

  for (size_t i = 0; i < bricks->size; i++) {
    Brick *a = array_get(bricks, i);
    int minz = min(a->start.z, a->end.z);
    if (minz == 1) {
      continue;
    }
    int max_fall_z = minz - 1;
    bool stop = false;
    bool has_fallen = false;
    for (int fz = 1; fz <= max_fall_z && !stop; fz++) {
      for (size_t j = 0; j < bricks->size && !stop; j++) {
        if (i == j) continue;
        Brick *b = array_get(bricks, j);
        stop = stop || collides(a, b, -1);
      }
      if (!stop) {
        has_fallen = true;
        a->start.z--;
        a->end.z--;
      }
    }
    if (has_fallen) {
      fallen[i] = 1;
    }
  }
  return fallen;
}

int part1(StringArray *lines) {
  Array *bricks = array_new(Brick);

  for (size_t l = 0; l < lines->size; l++) {
    char *line = lines->items[l];

    int sx, sy, sz, ex, ey, ez;
    if (sscanf(line, "%d,%d,%d~%d,%d,%d", &sx, &sy, &sz, &ex, &ey, &ez)) {
      array_push(bricks, &(Brick) { .start = { sx, sy, sz }, .end = { ex, ey, ez } });
    }
  }
  //debugf(brick_array_print, bricks);

  int falled = fall(bricks);
  debug("%d bricks falled\n", falled);
  //debugf(brick_array_print, bricks);
  while (falled != 0) falled = fall(bricks);

  bool b __attribute((unused)) = fixed(bricks);
  debug("fixed after fall: %s\n", b ? "true" : "false");

  int result = 0;
  for (size_t i = 0; i < bricks->size; i++) {
    Array *dup = array_dup(bricks);
    array_remove(dup, i);
    //debug("f = %d\n", f);
    if (fixed(dup)) {
      result++;
    }
    array_free(dup);
  }

  array_free(bricks);

  return result;
}

int part2(StringArray *lines) {
  Array *bricks = array_new(Brick);

  for (size_t l = 0; l < lines->size; l++) {
    char *line = lines->items[l];

    int sx, sy, sz, ex, ey, ez;
    if (sscanf(line, "%d,%d,%d~%d,%d,%d", &sx, &sy, &sz, &ex, &ey, &ez)) {
      array_push(bricks, &(Brick) { .start = { sx, sy, sz }, .end = { ex, ey, ez } });
    }
  }
  //debugf(brick_array_print, bricks);

  int falled = fall(bricks);
  debug("%d bricks falled\n", falled);
  //debugf(brick_array_print, bricks);
  while (falled != 0) falled = fall(bricks);

  bool b __attribute((unused)) = fixed(bricks);
  debug("fixed after fall: %s\n", b ? "true" : "false");

  int result = 0;
  for (size_t i = 0; i < bricks->size; i++) {
    Array *dup = array_dup(bricks);
    array_remove(dup, i);

    int *fallen = calloc(dup->size, sizeof(int));

    int *f = fall2(dup);
    int fs = false_sum(f, dup->size);
    for (size_t j = 0; j < dup->size; j++) {
      fallen[j] += f[j];
    }
    free(f);

    debug("%zu / %zu:\n", i, bricks->size);
    debug("\tf = %d\n", fs);
    while (fs > 0) {
      f = fall2(dup);
      fs = false_sum(f, dup->size);
      for (size_t j = 0; j < dup->size; j++) {
        fallen[j] += f[j];
      }
      free(f);
      debug("\tff = %d\n", fs);
    }

    result += false_sum(fallen, dup->size);

    array_free(dup);
    free(fallen);
  }

  array_free(bricks);

  return result;
}

int main(void) {
  test_case(day22, part1, sample, 5);
  test_case(day22, part1, data, 413);
  test_case(day22, part2, sample, 7);
  test_case(day22, part2, data, 41610);

  return 0;
}
