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
} Ordering;

i64 part1(StringArray *lines) {
  i64 result = 0;

  Array *pages_set = array_new(Array *);
  Array *ordering = array_new(Ordering);

  string_array_foreach (char *line, lines) {
    size_t line_len = strlen(line);

    i64 x, y;
    if (sscanf(line, "%lld|%lld", &x, &y) == 2) {
      array_push(ordering, &(Ordering){ .x = x, .y = y });
    } else if (line_len != 0) {
      StringArray *split = string_split(line, ",");
      Array *pages = string_atoll(split);
      array_push_pointer(pages_set, pages);

      string_array_free(split);
    }
  }

  for (size_t i = 0; i < pages_set->size; i++) {
    Array *pages = array_get_pointer(pages_set, i);
    debugf(i64_array_print, pages);

    bool ordered = true;
    for (size_t xi = 0; xi < pages->size; xi++) {
      for (size_t xj = xi; xj < pages->size; xj++) {
        if (xi != xj) {
          i64 x = i64_array_get(pages, xi);
          i64 y = i64_array_get(pages, xj);
          for (size_t k = 0; k < ordering->size; k++) {
            Ordering *o = array_get(ordering, k);
            if (o->x == y && o->y == x) {
              debug("%lld and %lld but %lld|%lld\n", x, y, o->x, o->y);
              ordered = false;
            }
          }
        }
      }
    }

    if (ordered) {
      debug("ordered\n");
      i64 m = i64_array_get(pages, pages->size / 2);
      result += m;
    } else {
      debug("not ordered\n");
    }
  }

  array_free(ordering);
  for (size_t i = 0; i < pages_set->size; i++) {
    Array *pages = array_get_pointer(pages_set, i);
    array_free(pages);
  }
  array_free(pages_set);

  return result;
}

i64 part2(StringArray *lines) {
  i64 result = 0;

  Array *pages_set = array_new(Array *);
  Array *ordering = array_new(Ordering);

  string_array_foreach (char *line, lines) {
    size_t line_len = strlen(line);
    debug("%s: %zu\n", line, line_len);

    i64 x, y;
    if (sscanf(line, "%lld|%lld", &x, &y) == 2) {
      debug("%lld <- %lld\n", x, y);
      array_push(ordering, &(Ordering){ .x = x, .y = y });
    } else if (line_len != 0) {
      StringArray *split = string_split(line, ",");
      Array *pages = string_atoll(split);
      array_push_pointer(pages_set, pages);

      string_array_free(split);
    }
  }

  for (size_t i = 0; i < pages_set->size; i++) {
    Array *pages = array_get_pointer(pages_set, i);
    debugf(i64_array_print, pages);

    bool ordered = true;
    for (size_t xi = 0; xi < pages->size; xi++) {
      for (size_t xj = xi; xj < pages->size; xj++) {
        if (xi != xj) {
          i64 x = i64_array_get(pages, xi);
          i64 y = i64_array_get(pages, xj);
          for (size_t k = 0; k < ordering->size; k++) {
            Ordering *o = array_get(ordering, k);
            if (o->x == y && o->y == x) {
              debug("%lld and %lld but %lld|%lld\n", x, y, o->x, o->y);
              array_set_rval(pages, xi, y);
              array_set_rval(pages, xj, x);
              ordered = false;
            }
          }
        }
      }
    }

    if (ordered) {
      debug("ordered\n");
    } else {
      debug("not ordered\n");
      debugf(i64_array_print, pages);
      i64 m = i64_array_get(pages, pages->size / 2);
      result += m;
    }
  }

  array_free(ordering);
  for (size_t i = 0; i < pages_set->size; i++) {
    Array *pages = array_get_pointer(pages_set, i);
    array_free(pages);
  }
  array_free(pages_set);

  return result;
}

int main(void) {
  test_case(day5, part1, sample, 143);
  test_case(day5, part1, data, 6260);
  test_case(day5, part2, sample, 123);
  test_case(day5, part2, data, 5346);

  return 0;
}
