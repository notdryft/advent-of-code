#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "string.h"

constexpr size_t BUFFER_LENGTH = 1024;

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
} Coord;

void coord_array_print(Array *array) {
  printf("Array{ capacity = %zu, size = %zu, stride = %zu, data = [ ", array->capacity, array->size, array->stride);
  for (size_t i = 0; i < array->size; i++) {
    Coord *coord = array_get(array, i);
    if (i == array->size - 1) {
      printf("(%zu, %zu)", coord->x, coord->y);
    } else {
      printf("(%zu, %zu), ", coord->x, coord->y);
    }
  }
  printf(" ] }\n");
}

// https://en.wikipedia.org/wiki/Shoelace_formula#Triangle_formula
long long polygon_area(Array *coords) {
  long long area = 0;
  for (size_t i = 0; i < coords->size; i++) {
    Coord *c1 = array_get(coords, i);
    Coord *c2 = i == coords->size - 1 ? array_get(coords, 0) : array_get(coords, i + 1);
    area += c1->x * c2->y - c2->x * c1->y;
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
  Array *coords = array_new(Coord);
  for (size_t i = 0; i < instructions->size; i++) {
    Instruction *instruction = array_get(instructions, i);

    Coord c = { .x = x, .y = y };
    array_push(coords, c);

    perimeter += instruction->distance;
    if (instruction->direction == R) x += instruction->distance;
    else if (instruction->direction == D) y += instruction->distance;
    else if (instruction->direction == L) x -= instruction->distance;
    else y -= instruction->distance;

    //printf("%zu %zu\n", x, y);
  }
  //coord_array_print(coords);

  long long area = polygon_area(coords);
  long long points = points_inside_polygon(area, perimeter);

  array_free(coords);

  return perimeter + points;
}

long long part1(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == nullptr) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  StringArray *lines = string_array_new();
  Array *instructions = array_new(Instruction);

  char buffer[BUFFER_LENGTH] = {};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    char direction_str[2];
    long long distance;

    if (sscanf(buffer, "%[RDLU] %lld (#[a-z0-9]+)", direction_str, &distance)) {
      enum Direction direction;
      if (direction_str[0] == 'R') direction = R;
      else if (direction_str[0] == 'D') direction = D; 
      else if (direction_str[0] == 'L') direction = L;
      else direction = U;

      Instruction instruction = { .direction = direction, .distance = distance };

      array_push(instructions, instruction);
    }
  }
  fclose(fp);
  string_array_free(lines);

  //instruction_array_print(instructions);

  long long area = area_from_instructions(instructions);
  printf("area = %lld\n", area);

  array_free(instructions);

  return area;
}

long long part2(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == nullptr) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  StringArray *lines = string_array_new();
  Array *instructions = array_new(Instruction);

  char buffer[BUFFER_LENGTH] = {};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    char color_str[7];

    if (sscanf(buffer, "%*s %*s (#%6s)", color_str)) {
      enum Direction direction = color_str[5] - '0';

      char distance_str[6];
      strncpy(distance_str, color_str, 5);
      distance_str[5] = '\0';
      size_t distance = strtol(distance_str, nullptr, 16);

      Instruction instruction = { .direction = direction, .distance = distance };

      array_push(instructions, instruction);
    }
  }
  fclose(fp);
  string_array_free(lines);

  //instruction_array_print(instructions);

  long long area = area_from_instructions(instructions);
  printf("area = %lld\n", area);

  array_free(instructions);

  return area;
}

int main(void) {
  assert(part1("../../inputs/2023/day18/sample") == 62);
  assert(part1("../../inputs/2023/day18/data") == 70253);
  assert(part2("../../inputs/2023/day18/sample") == 952408144115);
  assert(part2("../../inputs/2023/day18/data") == 131265059885080);

  return 0;
}
