#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "string.h"

#define BUFFER_LENGTH 1024

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

int traverse(char **cavern, size_t mx, size_t my, size_t sx, size_t sy, int direction) {
  int **beams = (int **) malloc(sizeof(int *) * my);
  for (size_t y = 0; y < my; y++) {
    beams[y] = (int *) malloc(sizeof(int) * (mx + 1));
    memset(beams[y], 0, sizeof(int) * mx);
    beams[y][mx] = '\0';
  }

  traverse_rec(cavern, beams, mx, my, sx, sy, direction);
  //debug_beams(beams, mx, my);
  //print_beams(beams, mx, my);

  int sum = 0;
  for (size_t y = 0; y < my; y++) {
    for (size_t x = 0; x < mx; x++) {
      if (beams[y][x] != 0) {
        sum++;
      }
    }
  }

  for (size_t y = 0; y < my; y++) {
    free(beams[y]);
  }
  free(beams);

  return sum;
}

int part1(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  StringArray *cavern = string_array_new();

  char buffer[BUFFER_LENGTH] = {0};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    string_array_push(cavern, buffer);
  }

  fclose(fp);

  string_array_print_raw(cavern);
  printf("\n");

  size_t mx = strlen(cavern->items[0]);
  size_t my = cavern->size;
  
  int direction = EAST;
  size_t sx = 0, sy = 0;

  int sum = traverse(cavern->items, mx, my, sx, sy, direction);
  printf("sum = %d\n", sum);

  string_array_free(cavern);

  return sum;
}

int part2(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  StringArray *cavern = string_array_new();

  char buffer[BUFFER_LENGTH] = {0};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    string_array_push(cavern, buffer);
  }

  fclose(fp);

  string_array_print_raw(cavern);
  printf("\n");

  size_t mx = strlen(cavern->items[0]);
  size_t my = cavern->size;

  int max = -1; 
  for (size_t sx = 0; sx < mx; sx++) {
    int sum = traverse(cavern->items, mx, my, sx, 0, SOUTH);
    max = sum > max ? sum : max;
  }
  for (size_t sy = 0; sy < my; sy++) {
    int sum = traverse(cavern->items, mx, my, mx - 1, sy, WEST);
    max = sum > max ? sum : max;
  }
  for (size_t sx = 0; sx < mx; sx++) {
    int sum = traverse(cavern->items, mx, my, sx, my - 1, NORTH);
    max = sum > max ? sum : max;
  }
  for (size_t sy = 0; sy < my; sy++) {
    int sum = traverse(cavern->items, mx, my, 0, sy, EAST);
    max = sum > max ? sum : max;
  }

  string_array_free(cavern);

  printf("max = %d\n", max);

  return max;
}

int main() {
  assert(part1("../../inputs/2023/day16/part1_test") == 46);
  assert(part1("../../inputs/2023/day16/data") == 6855);
  assert(part2("../../inputs/2023/day16/part2_test") == 51);
  assert(part2("../../inputs/2023/day16/data") == 7513);

  return 0;
}
