#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "algos.h"
#include "array.h"
#include "string.h"

constexpr size_t BUFFER_LENGTH = 1024;

size_t highest(StringArray *rocks, size_t x, size_t y) {
  while (y > 0 && rocks->items[y - 1][x] == '.') {
    y--;
  }
  return y;
}

size_t leftmost(StringArray *rocks, size_t x, size_t y) {
  while (x > 0 && rocks->items[y][x - 1] == '.') {
    x--;
  }
  return x;
}

size_t lowest(StringArray *rocks, size_t x, size_t y) {
  while (y < rocks->size - 1 && rocks->items[y + 1][x] == '.') {
    y++;
  }
  return y;
}

size_t rightmost(StringArray *rocks, size_t x, size_t y) {
  while (x < rocks->size - 1 && rocks->items[y][x + 1] == '.') {
    x++;
  }
  return x;
}

void swap(StringArray *rocks, size_t x1, size_t y1, size_t x2, size_t y2) {
  char tmp = rocks->items[y2][x2];
  rocks->items[y2][x2] = rocks->items[y1][x1];
  rocks->items[y1][x1] = tmp;
}

#define DIRECTIONS 4
#define NORTH 0
#define WEST 1
#define SOUTH 2
#define EAST 3

void tilt_nw(StringArray *rocks, size_t len, int direction) {
  for (size_t y = 0; y < rocks->size; y++) {
    for (size_t x = 0; x < len; x++) {
      if (rocks->items[y][x] == 'O') {
        size_t x2 = x, y2 = y;
        if (direction == NORTH) {
          y2 = highest(rocks, x, y);
        } else if (direction == WEST) {
          x2 = leftmost(rocks, x, y);
        }
        swap(rocks, x, y, x2, y2);
      }
    }
  }
}

void tilt_se(StringArray *rocks, size_t len, int direction) {
  for (long y = rocks->size - 1; y >= 0; y--) {
    for (long x = len - 1; x >= 0; x--) {
      if (rocks->items[y][x] == 'O') {
        size_t x2 = x, y2 = y;
        if (direction == SOUTH) {
          y2 = lowest(rocks, x, y);
        } else if (direction == EAST) {
          x2 = rightmost(rocks, x, y);
        }
        swap(rocks, x, y, x2, y2);
      }
    }
  }
}

void tilt(StringArray *rocks, size_t len, int direction) {
  if (direction == NORTH || direction == WEST) {
    tilt_nw(rocks, len, direction);
  } else {
    tilt_se(rocks, len, direction);
  }
}

int load(StringArray *rocks) {
  int sum = 0;
  for (size_t y = 0; y < rocks->size; y++) {
    for (size_t x = 0; x < strlen(rocks->items[y]); x++) {
      if (rocks->items[y][x] == 'O') {
        sum += (rocks->size - y);
      }
    }
  }
  return sum;
}

int part1(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == nullptr) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  StringArray *rocks = string_array_new();

  char buffer[BUFFER_LENGTH] = {};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';
    string_array_push(rocks, buffer);
  }

  fclose(fp);

  string_array_print_raw(rocks);
  printf("\n");

  tilt(rocks, strlen(rocks->items[0]), NORTH);
  string_array_print_raw(rocks);
  printf("\n");

  int sum = load(rocks);
  printf("sum = %d\n", sum);

  string_array_free(rocks);

  return sum;
}

int part2(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == nullptr) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  StringArray *rocks = string_array_new();

  char buffer[BUFFER_LENGTH] = {};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';
    string_array_push(rocks, buffer);
  }

  fclose(fp);

  string_array_print_raw(rocks);
  printf("\n");

  Array *loads = array_new(int);

  int cycles = 1000000000;
  printf("%d\n", cycles);
  Cycle *c = nullptr;
  size_t len = strlen(rocks->items[0]);
  for (int i = 0; i < cycles; i++) {
    for (int d = 0; d < DIRECTIONS; d++) {
      tilt(rocks, len, d);
    }
    //string_array_print_raw(rocks);
    //printf("\n");
    int l = load(rocks);
    array_push(loads, l);
    c = find_longest_cycle(loads, 2); 
    if (c != nullptr) {
      break;
    }
  }
  //string_array_print_raw(rocks);
  //printf("\n");
  int_array_print(loads);

  int sum = -1;
  if (c != nullptr) {
    int n = c->start + (cycles - 1 - c->start) % c->period;
    printf("start = %zu, period = %zu, result = %d\n", c->start, c->period, n);
    sum = int_array_get(loads, n);
  }

  printf("sum = %d\n", sum);

  array_free(loads);
  free(c);
  string_array_free(rocks);

  return sum;
}

int main(void) {
  assert(part1("../../inputs/2023/day14/sample") == 136);
  assert(part1("../../inputs/2023/day14/data") == 110274);
  assert(part2("../../inputs/2023/day14/sample") == 64);
  assert(part2("../../inputs/2023/day14/data") == 90982);

  return 0;
}
