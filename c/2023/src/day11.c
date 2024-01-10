#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "string.h"

#define BUFFER_LENGTH 1024

typedef struct {
  size_t ordinal;
  size_t x;
  size_t y;
} Galaxy;

typedef struct {
  Galaxy a;
  Galaxy b;
} Pair;

bool size_t_array_contains(size_t *array, size_t size, size_t expected) {
  for (size_t i = 0; i < size; i++) {
    if (array[i] == expected) {
      return true;
    }
  }
  return false;
}

bool string_all(char *str, char c) {
  bool all = true;
  for (size_t i = 0; i < strlen(str) && all; i++) {
    if (str[i] != c) {
      all = false;
    }
  }
  return all;
}

unsigned long long distance(Galaxy *a, Galaxy *b) {
  unsigned long long width = a->x > b->x ? a->x - b->x : b->x - a->x;
  unsigned long long height = a->y > b->y ? a->y - b->y : b->y - a->y;
  return width + height;
}

unsigned long long part1(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  StringArray *universe = string_array_new();
  size_t universe_width = 0;

  char buffer[BUFFER_LENGTH] = {0};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    string_array_push(universe, buffer);
    universe_width = strlen(buffer);
  }

  fclose(fp);

  //string_array_print_raw(universe);
  //printf("\n");

  size_t galaxies_size = 0;
  Galaxy *galaxies = malloc(sizeof(Galaxy) * 1000);

  size_t col_expands_size = 0;
  size_t *col_expands = malloc(sizeof(size_t) * 1000);

  for (size_t i = 0; i < universe_width; i++) {
    bool all = true;
    for (size_t j = 0; j < universe->size && all; j++) {
      char *row = string_array_get(universe, j);
      if (row[i] != '.') {
        all = false;
      }
    }
    if (all) {
      col_expands[col_expands_size++] = i;
    }
  }

  size_t row_expand = 0;
  for (size_t j = 0; j < universe->size; j++) {
    char *row = string_array_get(universe, j);
    size_t col_expand = 0;
    if (string_all(row, '.')) {
      row_expand++;
      //printf("expanding row %zu\n", j);
    } else {
      for (size_t i = 0; i < strlen(row); i++) {
        if (size_t_array_contains(col_expands, col_expands_size, i)) {
          col_expand++;
          //printf("expanding col %zu\n", i);
        }
        if (row[i] == '#') {
          Galaxy galaxy = { .ordinal = galaxies_size + 1, .x = i + col_expand, .y = j + row_expand };
          //printf("Galaxy { %zu: (%zu, %zu) }\n", galaxy.ordinal, galaxy.x, galaxy.y);
          galaxies[galaxies_size++] = galaxy;
        }
      }
    }
  }
  free(col_expands);
  string_array_free(universe);

  size_t p = galaxies_size * (galaxies_size - 1) / 2;
  printf("galaxies = %zu, pairs = %zu\n", galaxies_size, p);

  size_t pairs_size = 0;
  Pair *pairs = malloc(sizeof(Pair) * 10000);
  for (size_t i = 0; i < galaxies_size - 1; i++) {
    for (size_t j = i + 1; j < galaxies_size; j++) {
      Pair pair = { .a = galaxies[i], .b = galaxies[j] };
      pairs[pairs_size++] = pair;
      //printf("pairing { %zu: (%zu, %zu) } with  { %zu: (%zu, %zu) }\n", pair.a.ordinal, pair.a.x, pair.a.y, pair.b.ordinal, pair.b.x, pair.b.y);
    }
  }

  free(galaxies);

  printf("pairs = %zu\n", pairs_size);
  assert(pairs_size == p);

  unsigned long long sum = 0;

  for (size_t i = 0; i < pairs_size; i++) {
    Pair pair = pairs[i];
    unsigned long long d = distance(&pair.a, &pair.b);
    printf("Between galaxy %zu and galaxy %zu: %llu\n", pair.a.ordinal, pair.b.ordinal, d);
    sum += d;
  }

  printf("sum = %llu\n", sum);

  free(pairs);

  return sum;
}

unsigned long long part2(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  StringArray *universe = string_array_new();
  size_t universe_width = 0;

  char buffer[BUFFER_LENGTH] = {0};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    string_array_push(universe, buffer);
    universe_width = strlen(buffer);
  }

  fclose(fp);

  //string_array_print_raw(universe);
  //printf("\n");

  size_t galaxies_size = 0;
  Galaxy *galaxies = malloc(sizeof(Galaxy) * 1000);

  size_t col_expands_size = 0;
  size_t *col_expands = malloc(sizeof(size_t) * 1000);

  for (size_t i = 0; i < universe_width; i++) {
    bool all = true;
    for (size_t j = 0; j < universe->size && all; j++) {
      char *row = string_array_get(universe, j);
      if (row[i] != '.') {
        all = false;
      }
    }
    if (all) {
      col_expands[col_expands_size++] = i;
    }
  }

  size_t row_expand = 0;
  for (size_t j = 0; j < universe->size; j++) {
    char *row = string_array_get(universe, j);
    size_t col_expand = 0;
    if (string_all(row, '.')) {
      row_expand += 999999;
      //printf("expanding row %zu\n", j);
    } else {
      for (size_t i = 0; i < strlen(row); i++) {
        if (size_t_array_contains(col_expands, col_expands_size, i)) {
          col_expand += 999999;
          //printf("expanding col %zu\n", i);
        }
        if (row[i] == '#') {
          Galaxy galaxy = { .ordinal = galaxies_size + 1, .x = i + col_expand, .y = j + row_expand };
          //printf("Galaxy { %zu: (%zu, %zu) }\n", galaxy.ordinal, galaxy.x, galaxy.y);
          galaxies[galaxies_size++] = galaxy;
        }
      }
    }
  }
  free(col_expands);
  string_array_free(universe);

  size_t p = galaxies_size * (galaxies_size - 1) / 2;
  printf("galaxies = %zu, pairs = %zu\n", galaxies_size, p);

  size_t pairs_size = 0;
  Pair *pairs = malloc(sizeof(Pair) * 10000);
  for (size_t i = 0; i < galaxies_size - 1; i++) {
    for (size_t j = i + 1; j < galaxies_size; j++) {
      Pair pair = { .a = galaxies[i], .b = galaxies[j] };
      pairs[pairs_size++] = pair;
      //printf("pairing { %zu: (%zu, %zu) } with  { %zu: (%zu, %zu) }\n", pair.a.ordinal, pair.a.x, pair.a.y, pair.b.ordinal, pair.b.x, pair.b.y);
    }
  }

  free(galaxies);

  printf("pairs = %zu\n", pairs_size);
  assert(pairs_size == p);

  unsigned long long sum = 0;

  for (size_t i = 0; i < pairs_size; i++) {
    Pair pair = pairs[i];
    unsigned long long d = distance(&pair.a, &pair.b);
    //printf("Between galaxy %zu and galaxy %zu: %d\n", pair.a.ordinal, pair.b.ordinal, d);
    sum += d;
  }

  printf("sum = %llu\n", sum);

  free(pairs);

  return sum;
}

int main(void) {
  assert(part1("../../inputs/2023/day11/sample") == 374);
  assert(part1("../../inputs/2023/day11/data") == 10292708);
  assert(part2("../../inputs/2023/day11/sample") == 82000210);
  assert(part2("../../inputs/2023/day11/data") == 790194712336);

  return 0;
}
