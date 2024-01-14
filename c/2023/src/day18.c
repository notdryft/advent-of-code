#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "string.h"

enum Direction {
 R,
 D,
 L,
 U
};

typedef struct {
  enum Direction direction;
  long long distance;
} Instruction;

void instruction_array_print(Array *array) {
  printf("Array{ capacity = %zu, size = %zu, stride = %zu, data = [ ", array->capacity, array->size, array->stride);
  for (size_t i = 0; i < array->size; i++) {
    Instruction *instruction = array_get(array, i);
    if (i == array->size - 1) {
      printf("{ direction = %d, distance = %lld }", instruction->direction, instruction->distance);
    } else {
      printf("{ direction = %d, distance = %lld }, ", instruction->direction, instruction->distance);
    }
  }
  printf(" ] }\n");
}

typedef struct {
  size_t x;
  size_t y;
} Vec2;

void vec2_array_print(Array *array) {
  printf("Array{ capacity = %zu, size = %zu, stride = %zu, data = [ ", array->capacity, array->size, array->stride);
  for (size_t i = 0; i < array->size; i++) {
    Vec2 *v = array_get(array, i);
    if (i == array->size - 1) {
      printf("(%zu, %zu)", v->x, v->y);
    } else {
      printf("(%zu, %zu), ", v->x, v->y);
    }
  }
  printf(" ] }\n");
}

// https://en.wikipedia.org/wiki/Shoelace_formula#Triangle_formula
long long polygon_area(Array *vecs) {
  long long area = 0;
  for (size_t i = 0; i < vecs->size; i++) {
    Vec2 *u = array_get(vecs, i);
    Vec2 *v = i == vecs->size - 1 ? array_get(vecs, 0) : array_get(vecs, i + 1);
    area += u->x * v->y - v->x * u->y;
  }
  return area /= 2;
}

// https://en.wikipedia.org/wiki/Pick%27s_theorem
long long points_inside_polygon(long long area, long long perimeter) {
  return area - perimeter / 2 + 1;
}

long long area_from_instructions(Array *instructions) {
  long long x = 0, y = 0;
  long long perimeter = 0;
  Array *vecs = array_new(Vec2);
  for (size_t i = 0; i < instructions->size; i++) {
    Instruction *instruction = array_get(instructions, i);

    array_push(vecs, &(Vec2) { x, y });

    perimeter += instruction->distance;
    if (instruction->direction == R) x += instruction->distance;
    else if (instruction->direction == D) y += instruction->distance;
    else if (instruction->direction == L) x -= instruction->distance;
    else y -= instruction->distance;

    debug("%llu %llu\n", x, y);
  }
  debugf(vec2_array_print, vecs);

  long long area = polygon_area(vecs);
  long long points = points_inside_polygon(area, perimeter);

  array_free(vecs);

  return perimeter + points;
}

long long part1(StringArray *lines) {
  Array *instructions = array_new(Instruction);

  for (size_t l = 0; l < lines->size; l++) {
    char *line = lines->items[l];

    char direction_str[2];
    long long distance;

    if (sscanf(line, "%[RDLU] %lld (#[a-z0-9]+)", direction_str, &distance)) {
      enum Direction direction;
      if (direction_str[0] == 'R') direction = R;
      else if (direction_str[0] == 'D') direction = D;
      else if (direction_str[0] == 'L') direction = L;
      else direction = U;

      array_push(instructions, &(Instruction) { direction, distance });
    }
  }
  debugf(instruction_array_print, instructions);

  long long area = area_from_instructions(instructions);
  array_free(instructions);

  return area;
}

long long part2(StringArray *lines) {
  Array *instructions = array_new(Instruction);

  for (size_t l = 0; l < lines->size; l++) {
    char *line = lines->items[l];

    char color_str[7];

    if (sscanf(line, "%*s %*s (#%6s)", color_str)) {
      enum Direction direction = color_str[5] - '0';

      char distance_str[6];
      strncpy(distance_str, color_str, 5);
      distance_str[5] = '\0';
      size_t distance = strtol(distance_str, nullptr, 16);

      array_push(instructions, &(Instruction) { direction, distance });
    }
  }
  debugf(instruction_array_print, instructions);

  long long area = area_from_instructions(instructions);
  array_free(instructions);

  return area;
}

int main(void) {
  test_case(day18, part1, sample, 62);
  test_case(day18, part1, data, 70253);
  test_case(day18, part2, sample, 952408144115);
  test_case(day18, part2, data, 131265059885080);

  return 0;
}
