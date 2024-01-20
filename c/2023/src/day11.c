#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "string.h"

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

unsigned long long part1(StringArray *lines) {
  size_t universe_width = strlen(lines->items[0]);
  //debugf(string_array_print_raw, lines);

  size_t galaxies_size = 0;
  Galaxy *galaxies = malloc(sizeof(*galaxies) * 1000);

  size_t col_expands_size = 0;
  size_t *col_expands = malloc(sizeof(*col_expands) * 1000);

  for (size_t i = 0; i < universe_width; i++) {
    bool all = true;
    for (size_t j = 0; j < lines->size && all; j++) {
      char *row = string_array_get(lines, j);
      if (row[i] != '.') {
        all = false;
      }
    }
    if (all) {
      col_expands[col_expands_size++] = i;
    }
  }

  size_t row_expand = 0;
  for (size_t j = 0; j < lines->size; j++) {
    char *row = string_array_get(lines, j);
    size_t col_expand = 0;
    if (string_all(row, '.')) {
      row_expand++;
      debug("expanding row %zu\n", j);
    } else {
      for (size_t i = 0; i < strlen(row); i++) {
        if (size_t_array_contains(col_expands, col_expands_size, i)) {
          col_expand++;
          debug("expanding col %zu\n", i);
        }
        if (row[i] == '#') {
          Galaxy galaxy = { .ordinal = galaxies_size + 1, .x = i + col_expand, .y = j + row_expand };
          debug("Galaxy { %zu: (%zu, %zu) }\n", galaxy.ordinal, galaxy.x, galaxy.y);
          galaxies[galaxies_size++] = galaxy;
        }
      }
    }
  }
  free(col_expands);

  size_t p = galaxies_size * (galaxies_size - 1) / 2;
  debug("galaxies = %zu, pairs = %zu\n", galaxies_size, p);

  size_t pairs_capacity = 10000;
  size_t pairs_size = 0;
  Pair *pairs = malloc(sizeof(*pairs) * pairs_capacity);
  for (size_t i = 0; i < galaxies_size - 1; i++) {
    for (size_t j = i + 1; j < galaxies_size; j++) {
      Pair pair = { .a = galaxies[i], .b = galaxies[j] };
      if (pairs_size + 1 > pairs_capacity) {
        pairs_capacity *= 2;
        pairs = realloc(pairs, sizeof(*pairs) * pairs_capacity);
      }
      pairs[pairs_size++] = pair;
      debug("pairing { %zu: (%zu, %zu) } with  { %zu: (%zu, %zu) }\n", pair.a.ordinal, pair.a.x, pair.a.y, pair.b.ordinal, pair.b.x, pair.b.y);
    }
  }

  free(galaxies);

  debug("pairs = %zu\n", pairs_size);
  assert(pairs_size == p);

  unsigned long long result = 0;
  for (size_t i = 0; i < pairs_size; i++) {
    Pair pair = pairs[i];
    unsigned long long d = distance(&pair.a, &pair.b);
    debug("Between galaxy %zu and galaxy %zu: %llu\n", pair.a.ordinal, pair.b.ordinal, d);
    result += d;
  }

  free(pairs);

  return result;
}

unsigned long long part2(StringArray *lines) {
  size_t universe_width = strlen(lines->items[0]);
  //debugf(string_array_print_raw, lines);

  size_t galaxies_size = 0;
  Galaxy *galaxies = malloc(sizeof(*galaxies) * 1000);

  size_t col_expands_size = 0;
  size_t *col_expands = malloc(sizeof(*col_expands) * 1000);

  for (size_t i = 0; i < universe_width; i++) {
    bool all = true;
    for (size_t j = 0; j < lines->size && all; j++) {
      char *row = string_array_get(lines, j);
      if (row[i] != '.') {
        all = false;
      }
    }
    if (all) {
      col_expands[col_expands_size++] = i;
    }
  }

  size_t row_expand = 0;
  for (size_t j = 0; j < lines->size; j++) {
    char *row = string_array_get(lines, j);
    size_t col_expand = 0;
    if (string_all(row, '.')) {
      row_expand += 999999;
      debug("expanding row %zu\n", j);
    } else {
      for (size_t i = 0; i < strlen(row); i++) {
        if (size_t_array_contains(col_expands, col_expands_size, i)) {
          col_expand += 999999;
          debug("expanding col %zu\n", i);
        }
        if (row[i] == '#') {
          Galaxy galaxy = { .ordinal = galaxies_size + 1, .x = i + col_expand, .y = j + row_expand };
          debug("Galaxy { %zu: (%zu, %zu) }\n", galaxy.ordinal, galaxy.x, galaxy.y);
          galaxies[galaxies_size++] = galaxy;
        }
      }
    }
  }
  free(col_expands);

  size_t p = galaxies_size * (galaxies_size - 1) / 2;
  debug("galaxies = %zu, pairs = %zu\n", galaxies_size, p);

  size_t pairs_capacity = 10000;
  size_t pairs_size = 0;
  Pair *pairs = malloc(sizeof(*pairs) * pairs_capacity);
  for (size_t i = 0; i < galaxies_size - 1; i++) {
    for (size_t j = i + 1; j < galaxies_size; j++) {
      Pair pair = { .a = galaxies[i], .b = galaxies[j] };
      if (pairs_size + 1 > pairs_capacity) {
        pairs_capacity *= 2;
        pairs = realloc(pairs, sizeof(*pairs) * pairs_capacity);
      }
      pairs[pairs_size++] = pair;
      debug("pairing { %zu: (%zu, %zu) } with  { %zu: (%zu, %zu) }\n", pair.a.ordinal, pair.a.x, pair.a.y, pair.b.ordinal, pair.b.x, pair.b.y);
    }
  }

  free(galaxies);

  debug("pairs = %zu\n", pairs_size);
  assert(pairs_size == p);

  unsigned long long result = 0;
  for (size_t i = 0; i < pairs_size; i++) {
    Pair pair = pairs[i];
    unsigned long long d = distance(&pair.a, &pair.b);
    debug("Between galaxy %zu and galaxy %zu: %llu\n", pair.a.ordinal, pair.b.ordinal, d);
    result += d;
  }

  free(pairs);

  return result;
}

int main(void) {
  test_case(day11, part1, sample, 374);
  test_case(day11, part1, data, 10292708);
  test_case(day11, part2, sample, 82000210);
  test_case(day11, part2, data, 790194712336);

  return 0;
}
