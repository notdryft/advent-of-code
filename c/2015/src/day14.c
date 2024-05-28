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
  // part 1
  i64 speed;
  i64 duration;
  i64 rest;
  // part 2
  i64 distance;
  i64 resting;
  i64 running;
  i64 points;
} Reindeer;

i64 part1(StringArray *lines, i64 steps) {
  Array *reindeers = array_new(Reindeer);

  string_array_foreach (char *line, lines) {
    i64 speed, duration, rest;
    if (sscanf(line, "%*s can fly %lld km/s for %lld seconds, but then must rest for %lld seconds.", &speed, &duration, &rest)) {
      array_push(reindeers, &(Reindeer) {
        .speed = speed,
        .duration = duration,
        .rest = rest
      });
    }
  }

  i64 result = -1;
  array_foreach (Reindeer *reindeer, reindeers) {
    i64 running = reindeer->duration;
    i64 resting = 0;

    i64 distance = 0;
    for (i64 step = 0; step < steps; step++) {
      if (running > 0) {
        running--;
        distance += reindeer->speed;
        if (running == 0) {
          resting = reindeer->rest;
        }
      } else if (resting > 0) {
        resting--;
        if (resting == 0) {
          running = reindeer->duration;
        }
      }
    }

    if (distance > result) {
      result = distance;
    }
  }

  return result;
}

i64 part2(StringArray *lines, i64 steps) {
  Array *reindeers = array_new(Reindeer);

  string_array_foreach (char *line, lines) {
    i64 speed, duration, rest;
    if (sscanf(line, "%*s can fly %lld km/s for %lld seconds, but then must rest for %lld seconds.", &speed, &duration, &rest)) {
      array_push(reindeers, &(Reindeer) {
        .speed = speed,
        .duration = duration,
        .rest = rest,
        .distance = 0,
        .resting = 0,
        .running = duration,
        .points = 0
      });
    }
  }

  for (i64 step = 0; step < steps; step++) {
    debug("step %lld / %lld\n", step + 1, steps);

    i64 leading_distance = -1;
    array_foreach (Reindeer *reindeer, reindeers) {
      if (reindeer->running > 0) {
        reindeer->running--;
        reindeer->distance += reindeer->speed;
        if (reindeer->running == 0) {
          reindeer->resting = reindeer->rest;
        }
      } else if (reindeer->resting > 0) {
        reindeer->resting--;
        if (reindeer->resting == 0) {
          reindeer->running = reindeer->duration;
        }
      }
      if (reindeer->distance > leading_distance) {
        leading_distance = reindeer->distance;
      }
    }
    for (size_t i = 0; i < reindeers->size; i++) {
      Reindeer *reindeer = array_get(reindeers, i);
      if (reindeer->distance == leading_distance) {
        reindeer->points++;
      }
      debug("  %zu: %lld (%lld)\n", i, reindeer->distance, reindeer->points);
    }
  }

  i64 result = -1;
  array_foreach (Reindeer *reindeer, reindeers) {
    if (reindeer->points > result) {
      result = reindeer->points;
    }
  }

  return result;
}

int main(void) {
  test_case(day14, part1, sample, 1120, 1000);
  test_case(day14, part1, data, 2640, 2503);
  test_case(day14, part2, sample, 689, 1000);
  test_case(day14, part2, data, 1102, 2503);

  return 0;
}
