#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

#define LIGHTS_SIZE 1000

i64 part1(StringArray *lines) {
  bool lights[LIGHTS_SIZE][LIGHTS_SIZE] = {};

  string_array_foreach (char *line, lines) {
    size_t x1, y1, x2, y2;
    if (sscanf(line, "turn off %zu,%zu through %zu,%zu", &x1, &y1, &x2, &y2)) {
      for (size_t j = y1; j <= y2; j++) {
        for (size_t i = x1; i <= x2; i++) {
          lights[j][i] = false;
        }
      }
    } else if (sscanf(line, "turn on %zu,%zu through %zu,%zu", &x1, &y1, &x2, &y2)) {
      for (size_t j = y1; j <= y2; j++) {
        for (size_t i = x1; i <= x2; i++) {
          lights[j][i] = true;
        }
      }
    } else if (sscanf(line, "toggle %zu,%zu through %zu,%zu", &x1, &y1, &x2, &y2)) {
      for (size_t j = y1; j <= y2; j++) {
        for (size_t i = x1; i <= x2; i++) {
          lights[j][i] = !lights[j][i];
        }
      }
    }
  }

  i64 result = 0;
  for (size_t j = 0; j < LIGHTS_SIZE; j++) {
    for (size_t i = 0; i < LIGHTS_SIZE; i++) {
      if (lights[j][i]) {
        result++;
      }
    }
  }

  return result;
}

i64 part2(StringArray *lines) {
  u32 lights[LIGHTS_SIZE][LIGHTS_SIZE] = {};

  string_array_foreach (char *line, lines) {
    size_t x1, y1, x2, y2;
    if (sscanf(line, "turn off %zu,%zu through %zu,%zu", &x1, &y1, &x2, &y2)) {
      for (size_t j = y1; j <= y2; j++) {
        for (size_t i = x1; i <= x2; i++) {
          if (lights[j][i] > 0) {
            lights[j][i] -= 1;
          }
        }
      }
    } else if (sscanf(line, "turn on %zu,%zu through %zu,%zu", &x1, &y1, &x2, &y2)) {
      for (size_t j = y1; j <= y2; j++) {
        for (size_t i = x1; i <= x2; i++) {
          lights[j][i] += 1;
        }
      }
    } else if (sscanf(line, "toggle %zu,%zu through %zu,%zu", &x1, &y1, &x2, &y2)) {
      for (size_t j = y1; j <= y2; j++) {
        for (size_t i = x1; i <= x2; i++) {
          lights[j][i] += 2;
        }
      }
    }
  }

  i64 result = 0;
  for (size_t j = 0; j < LIGHTS_SIZE; j++) {
    for (size_t i = 0; i < LIGHTS_SIZE; i++) {
      result += lights[j][i];
    }
  }

  return result;
}

int main(void) {
  test_case(day6, part1, data, 377891);
  test_case(day6, part2, data, 14110788);

  return 0;
}
