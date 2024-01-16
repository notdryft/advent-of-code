#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "string.h"

#define NORTH 1
#define WEST 2
#define SOUTH 4
#define EAST 8

#define mask_contains(mask, direction) ((mask & direction) != 0)

void debug_beams(int **beams, size_t mx, size_t my) {
  for (size_t y = 0; y < my; y++) {
    for (size_t x = 0; x < mx; x++) {
      if (beams[y][x] == 0) {
        printf("   ");
      } else {
        printf("%3d", beams[y][x]);
      }
    }
    printf("\n");
  }
  printf("\n");
}

void print_beams(int **beams, size_t mx, size_t my) {
  for (size_t y = 0; y < my; y++) {
    for (size_t x = 0; x < mx; x++) {
      printf("%c", beams[y][x] == 0 ? '.' : '#');
    }
    printf("\n");
  }
  printf("\n");
}

void traverse_rec(char **cavern, int **beams, size_t mx, size_t my, size_t x, size_t y, int direction) {
  beams[y][x] |= direction;

  if (cavern[y][x] == '/') {
    if (direction == NORTH) direction = EAST;
    else if (direction == WEST) direction = SOUTH;
    else if (direction == SOUTH) direction = WEST;
    else if (direction == EAST) direction = NORTH;
  } else if (cavern[y][x] == '\\') {
    if (direction == NORTH) direction = WEST;
    else if (direction == WEST) direction = NORTH;
    else if (direction == SOUTH) direction = EAST;
    else if (direction == EAST) direction = SOUTH;
  } else if (cavern[y][x] == '|') {
    if (direction == WEST || direction == EAST) direction = NORTH | SOUTH;
  } else if (cavern[y][x] == '-') {
    if (direction == NORTH || direction == SOUTH) direction = WEST | EAST;
  }

  if (mask_contains(direction, NORTH) && y > 0 && !mask_contains(beams[y - 1][x], NORTH)) {
    traverse_rec(cavern, beams, mx, my, x, y - 1, NORTH);
  }
  if (mask_contains(direction, WEST) && x > 0 && !mask_contains(beams[y][x - 1], WEST)) {
    traverse_rec(cavern, beams, mx, my, x - 1, y, WEST);
  }
  if (mask_contains(direction, SOUTH) && y + 1 < my && !mask_contains(beams[y + 1][x], SOUTH)) {
    traverse_rec(cavern, beams, mx, my, x, y + 1, SOUTH);
  }
  if (mask_contains(direction, EAST) && x + 1 < mx && !mask_contains(beams[y][x + 1], EAST)) {
    traverse_rec(cavern, beams, mx, my, x + 1, y, EAST);
  }
}

int traverse(char **cavern, int **beams, size_t mx, size_t my, size_t sx, size_t sy, int direction) {
  for (size_t y = 0; y < my; y++) {
    memset(beams[y], 0, sizeof(int) * mx);
  }

  traverse_rec(cavern, beams, mx, my, sx, sy, direction);
  //debugf(debug_beams, beams, mx, my);
  //debugf(print_beams, beams, mx, my);

  int sum = 0;
  for (size_t y = 0; y < my; y++) {
    for (size_t x = 0; x < mx; x++) {
      if (beams[y][x] != 0) {
        sum++;
      }
    }
  }

  return sum;
}

int part1(StringArray *lines) {
  debugf(string_array_print_raw, lines);
  debug("\n");

  size_t mx = strlen(lines->items[0]);
  size_t my = lines->size;

  int direction = EAST;
  size_t sx = 0, sy = 0;

  int **beams = malloc(sizeof(*beams) * my);
  for (size_t y = 0; y < my; y++) {
    beams[y] = calloc(mx, sizeof(**beams));
  }

  int result = traverse(lines->items, beams, mx, my, sx, sy, direction);

  for (size_t y = 0; y < my; y++) {
    free(beams[y]);
  }
  free(beams);

  return result;
}

int part2(StringArray *lines) {
  debugf(string_array_print_raw, lines);
  debug("\n");

  size_t mx = strlen(lines->items[0]);
  size_t my = lines->size;

  int **beams = malloc(sizeof(*beams) * my);
  for (size_t y = 0; y < my; y++) {
    beams[y] = calloc(mx, sizeof(**beams));
  }

  int result = -1;
  for (size_t sx = 0; sx < mx; sx++) {
    int sum = traverse(lines->items, beams, mx, my, sx, 0, SOUTH);
    result = max(sum, result);
  }
  for (size_t sy = 0; sy < my; sy++) {
    int sum = traverse(lines->items, beams, mx, my, mx - 1, sy, WEST);
    result = max(sum, result);
  }
  for (size_t sx = 0; sx < mx; sx++) {
    int sum = traverse(lines->items, beams, mx, my, sx, my - 1, NORTH);
    result = max(sum, result);
  }
  for (size_t sy = 0; sy < my; sy++) {
    int sum = traverse(lines->items, beams, mx, my, 0, sy, EAST);
    result = max(sum, result);
  }

  for (size_t y = 0; y < my; y++) {
    free(beams[y]);
  }
  free(beams);

  return result;
}

int main(void) {
  test_case(day16, part1, sample, 46);
  test_case(day16, part1, data, 6855);
  test_case(day16, part2, sample, 51);
  test_case(day16, part2, data, 7513);

  return 0;
}
