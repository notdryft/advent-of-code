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
  i64 x;
  i64 y;
} Vec2;

typedef struct {
  Vec2 p;
  Vec2 v;
} Robot;

void robots_array_print(Array *array) {
  printf("Array{ capacity = %zu, size = %zu, stride = %zu, data = [ ", array->capacity, array->size, array->stride);
  for (size_t i = 0; i < array->size; i++) {
    Robot *robot = array_get(array, i);
    if (i == array->size - 1) {
      printf("{ p = (%lld, %lld), v = (%lld, %lld) }", robot->p.x, robot->p.y, robot->v.x, robot->v.y);
    } else {
      printf("{ p = (%lld, %lld), v = (%lld, %lld) }, ", robot->p.x, robot->p.y, robot->v.x, robot->v.y);
    }
  }
  printf(" ] }\n");
}

void print_grid(Array *robots, size_t cols, size_t rows) {
  for (size_t j = 0; j < rows; j++) {
    for (size_t i = 0; i < cols; i++) {
      size_t r = 0;
      array_foreach(Robot *robot, robots) {
        if ((size_t) robot->p.x == i && (size_t) robot->p.y == j) {
          r++;
        }
      }
      if (r > 0) {
        printf("%zu", r);
      } else {
        printf(".");
      }
    }
    printf("\n");
  }
}

i64 in_quadrant(Array *robots, size_t si, size_t sj, size_t ei, size_t ej) {
  i64 result = 0;
  array_foreach(Robot *robot, robots) {
    size_t px = robot->p.x, py = robot->p.y;
    if (si <= px && px < ei && sj <= py && py < ej) {
      result++;
    }
  }
  return result;
}

i64 part1(StringArray *lines, size_t cols, size_t rows) {
  Array *robots = array_new(Robot);
  string_array_foreach (char *line, lines) {
    Vec2 p, v;
    if (sscanf(line, "p=%lld,%lld v=%lld,%lld", &p.x, &p.y, &v.x, &v.y) == 4) {
      trace("%lld %lld %lld %lld\n", p.x, p.y, v.x, v.y);
      array_push(robots, &(Robot) { .p = p, .v = v });
    }
  }
  debugf(robots_array_print, robots);
  debug("before: \n");
  debugf(print_grid, robots, cols, rows);

  size_t seconds = 0;
  while (seconds < 100) {
    for (size_t i = 0; i < robots->size; i++) {
      Robot *robot = array_get(robots, i);
      robot->p.x += robot->v.x;
      if (robot->p.x < 0) robot->p.x += cols;
      if (robot->p.x >= (i64) cols) robot->p.x -= cols;
      robot->p.y += robot->v.y;
      if (robot->p.y < 0) robot->p.y += rows;
      if (robot->p.y >= (i64) rows) robot->p.y -= rows;
    }
    seconds++;
  }
  debug("after: \n");
  debugf(print_grid, robots, cols, rows);

  size_t mcols = cols / 2;
  size_t mrows = rows / 2;
  i64 q1 = in_quadrant(robots, 0, 0, mcols, mrows);
  i64 q2 = in_quadrant(robots, mcols + 1, 0, cols, mrows);
  i64 q3 = in_quadrant(robots, 0, mrows + 1, mcols, rows);
  i64 q4 = in_quadrant(robots, mcols + 1, mrows + 1, cols, rows);
  debug("%lld %lld %lld %lld\n", q1, q2, q3, q4);
  i64 result = q1 * q2 * q3 * q4;

  array_free(robots);

  return result;
}

bool has_xmas_tree(Array *robots) {
  size_t si = 55, ei = 85;
  size_t sj = 55, ej = 55;
  
  i64 result = 0;
  array_foreach(Robot *robot, robots) {
    size_t px = robot->p.x, py = robot->p.y;
    if (si <= px && px <= ei && sj <= py && py <= ej) {
      result++;
    }
  }
  return result == 31;
}

i64 part2(StringArray *lines, size_t cols, size_t rows) {
  Array *robots = array_new(Robot);
  string_array_foreach (char *line, lines) {
    Vec2 p, v;
    if (sscanf(line, "p=%lld,%lld v=%lld,%lld", &p.x, &p.y, &v.x, &v.y) == 4) {
      trace("%lld %lld %lld %lld\n", p.x, p.y, v.x, v.y);
      array_push(robots, &(Robot) { .p = p, .v = v });
    }
  }
  tracef(robots_array_print, robots);
  debug("before: \n");
  debugf(print_grid, robots, cols, rows);

  i64 seconds = 0;
  while (seconds < 100000) {
    for (size_t i = 0; i < robots->size; i++) {
      Robot *robot = array_get(robots, i);
      robot->p.x += robot->v.x;
      if (robot->p.x < 0) robot->p.x += cols;
      if (robot->p.x >= (i64) cols) robot->p.x -= cols;
      robot->p.y += robot->v.y;
      if (robot->p.y < 0) robot->p.y += rows;
      if (robot->p.y >= (i64) rows) robot->p.y -= rows;
    }
    seconds++;
    if (has_xmas_tree(robots)) {
      debug("after %lld seconds:\n", seconds);
      debugf(print_grid, robots, cols, rows);
      break;
    }
  }

  array_free(robots);

  return seconds;
}

int main(void) {
  test_case(day14, part1, sample, 12, 11, 7);
  test_case(day14, part1, data, 220971520, 101, 103);
  test_case(day14, part2, data, 6355, 101, 103);

  return 0;
}
