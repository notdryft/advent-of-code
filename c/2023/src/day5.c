#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "string.h"

constexpr size_t BUFFER_LENGTH = 1024;

enum AlmanachEntryType {
  SEED_TO_SOIL,
  SOIL_TO_FERTILIZER,
  FERTILIZER_TO_WATER,
  WATER_TO_LIGHT,
  LIGHT_TO_TEMPERATURE,
  TEMPERATURE_TO_HUMIDITY,
  HUMIDITY_TO_LOCATION,
  NUMBER_OF_ALMANACH_ENTRY_TYPES
};

typedef struct {
  ll start;
  ll end;
} Range;

inline bool range_contains(Range range, ll value) {
  return range.start <= value && value < range.end;
}

int range_cmp(const void *a, const void *b) {
  return ((Range *)a)->start < ((Range *)b)->start ? -1 : 1;
}

typedef struct {
  enum AlmanachEntryType source_type;
  Range range;
  ll offset;
} AlmanachEntry;

// [range                      )
//         [entry       )
// [before)[intersection)[after)
void apply_range(Array *waiting, Array *done, AlmanachEntry *entry, Range *range) {
  Range before = { range->start, min(range->end, entry->range.start) };
  Range intersection = { max(range->start, entry->range.start), min(range->end, entry->range.end) };
  Range after = { max(range->start, entry->range.end), range->end};

  if (before.start < before.end) {
    array_push(waiting, &before);
  }
  if (intersection.start < intersection.end) {
    array_push(done, &(Range) { intersection.start + entry->offset, intersection.end + entry->offset });
  }
  if (after.start < after.end) {
    array_push(waiting, &after);
  }
}

int entry_cmp(const void *a, const void *b) {
  const AlmanachEntry *ea = a, *eb = b;
  if (ea->source_type == eb->source_type) {
    return ea->range.start < eb->range.start ? -1 : 1;
  }
  return ea->source_type < eb->source_type ? -1 : 1;
}

ll part1(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == nullptr) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  Array *seeds = array_new(ll);
  Array *entries = array_new(AlmanachEntry);

  char source_type_str[BUFFER_LENGTH];
  enum AlmanachEntryType source_type = 0;

  char buffer[BUFFER_LENGTH] = {};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    if (buffer[0] == '\0') {
      debug("reset state\n\n");
      continue;
    }

    if (strncmp(buffer, "seeds: ", 6) == 0) {
      char *seeds_str = substring(buffer, 7, buffer_len - 7 - 1);
      StringArray *split = string_split(seeds_str, " ");
      for (size_t i = 0; i < split->size; i++) {
        array_push_rval(seeds, strtoll(split->items[i], nullptr, 10));
      }

      free(seeds_str);
      string_array_free(split);
    } else if (
      'a' <= buffer[0] && buffer[0] <= 'z' &&
      sscanf(buffer, "%[^-]-to-%*s map:", source_type_str)
    ) {
      if (strcmp(source_type_str, "seed") == 0) {
        source_type = SEED_TO_SOIL;
      } else if (strcmp(source_type_str, "soil") == 0) {
        source_type = SOIL_TO_FERTILIZER;
      } else if (strcmp(source_type_str, "fertilizer") == 0) {
        source_type = FERTILIZER_TO_WATER;
      } else if (strcmp(source_type_str, "water") == 0) {
        source_type = WATER_TO_LIGHT;
      } else if (strcmp(source_type_str, "light") == 0) {
        source_type = LIGHT_TO_TEMPERATURE;
      } else if (strcmp(source_type_str, "temperature") == 0) {
        source_type = TEMPERATURE_TO_HUMIDITY;
      } else if (strcmp(source_type_str, "humidity") == 0) {
        source_type = HUMIDITY_TO_LOCATION;
      }
      debug("new source type: \033[97;4m%s\033[0m\n", source_type_str);
    } else if ('0' <= buffer[0] && buffer[0] <= '9') {
      char *p;
      ll destination = strtoll(buffer, &p, 10);
      ll source = strtoll(p + 1, &p, 10);
      ll range = strtoll(p + 1, nullptr, 10);
      debug("entry for source type \033[97;1m%s\033[0m: %lld %lld %lld\n", source_type_str, destination, source, range);

      AlmanachEntry entry = {
        .source_type = source_type,
        .range = {
          .start = source,
          .end = source + range
        },
        .offset = destination - source
      };
      array_push(entries, &entry);
    }
  }
  fclose(fp);

  ll min = LLONG_MAX;
  for (size_t i = 0; i < seeds->size; i++) {
    ll seed_to_location = ll_array_get(seeds, i);
    for (size_t entry_type = 0; entry_type < NUMBER_OF_ALMANACH_ENTRY_TYPES; entry_type++) {
      bool stop = false;
      for (size_t j = 0; j < entries->size && !stop; j++) {
        AlmanachEntry *entry = array_get(entries, j);
        if (entry->source_type == entry_type) {
          if (range_contains(entry->range, seed_to_location)) {
            seed_to_location += entry->offset;
            stop = true;
          }
        }
      }
    }
    if (seed_to_location < min) {
      min = seed_to_location;
    }
  }

  printf("min = %lld\n", min);

  array_free(entries);
  array_free(seeds);

  return min;
}

ll part2(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == nullptr) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  Array *pairs = array_new(ll);
  Array *entries = array_new(AlmanachEntry);

  char source_type_str[BUFFER_LENGTH];
  enum AlmanachEntryType source_type = 0;

  char buffer[BUFFER_LENGTH] = {};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    if (buffer[0] == '\0') {
      debug("reset state\n\n");
      continue;
    }

    if (strncmp(buffer, "seeds: ", 6) == 0) {
      char *pairs_str = substring(buffer, 7, buffer_len - 7 - 1);
      StringArray *split = string_split(pairs_str, " ");
      for (size_t i = 0; i < split->size; i++) {
        array_push_rval(pairs, strtoll(split->items[i], nullptr, 0));
      }

      free(pairs_str);
      string_array_free(split);
    } else if (
      'a' <= buffer[0] && buffer[0] <= 'z' &&
      sscanf(buffer, "%[^-]-to-%*s map:", source_type_str)
    ) {
      if (strcmp(source_type_str, "seed") == 0) {
        source_type = SEED_TO_SOIL;
      } else if (strcmp(source_type_str, "soil") == 0) {
        source_type = SOIL_TO_FERTILIZER;
      } else if (strcmp(source_type_str, "fertilizer") == 0) {
        source_type = FERTILIZER_TO_WATER;
      } else if (strcmp(source_type_str, "water") == 0) {
        source_type = WATER_TO_LIGHT;
      } else if (strcmp(source_type_str, "light") == 0) {
        source_type = LIGHT_TO_TEMPERATURE;
      } else if (strcmp(source_type_str, "temperature") == 0) {
        source_type = TEMPERATURE_TO_HUMIDITY;
      } else if (strcmp(source_type_str, "humidity") == 0) {
        source_type = HUMIDITY_TO_LOCATION;
      }
      debug("new source type: \033[97;4m%s\033[0m\n", source_type_str);
    } else if ('0' <= buffer[0] && buffer[0] <= '9') {
      char *p;
      ll destination = strtoll(buffer, &p, 10);
      ll source = strtoll(p + 1, &p, 10);
      ll range = strtoll(p + 1, nullptr, 10);
      debug("entry for source type \033[97;1m%s\033[0m: %lld %lld %lld\n", source_type_str, destination, source, range);

      AlmanachEntry entry = {
        .source_type = source_type,
        .range = {
          .start = source,
          .end = source + range
        },
        .offset = destination - source
      };
      array_push(entries, &entry);
    }
  }
  fclose(fp);

  Array *ranges = array_new(Range);
  for (size_t i = 0; i < pairs->size; i += 2) {
    ll start = llu_array_get(pairs, i);
    ll r = llu_array_get(pairs, i + 1);
    array_push(ranges, &(Range) { start, start + r });
  }

  for (size_t entry_type = 0; entry_type < NUMBER_OF_ALMANACH_ENTRY_TYPES; entry_type++) {
    Array *done = array_new(Range);
    for (size_t i = 0; i < entries->size; i++) {
      AlmanachEntry *entry = array_get(entries, i);
      if (entry->source_type == entry_type) {
        Array *waiting = array_new(Range);
        while (ranges->size > 0) {
          Range *r = array_pop(ranges);
          apply_range(waiting, done, entry, r);
        }
        free(ranges);
        ranges = waiting;
      }
    }
    Array *tmp = array_concat(done, ranges);
    free(done);
    free(ranges);
    ranges = tmp;
  }

  ll min = LLONG_MAX;
  for (size_t i = 0; i < ranges->size; i++) {
    Range *r = array_get(ranges, i);
    if (r->start < min) {
      min = r->start;
    }
  }
  printf("min = %lld\n", min);

  array_free(entries);
  array_free(pairs);
  array_free(ranges);

  return min;
}

int main(void) {
  assert(part1("../../inputs/2023/day5/sample") == 35);
  assert(part1("../../inputs/2023/day5/data") == 403695602);
  assert(part2("../../inputs/2023/day5/sample") == 46);
  assert(part2("../../inputs/2023/day5/data") == 219529182);

  return 0;
}
