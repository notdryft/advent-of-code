#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "string.h"

#define BUFFER_LENGTH 1024

typedef unsigned long long llu;

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
  enum AlmanachEntryType source_type;
  llu destination;
  llu source;
  llu range;
} AlmanachEntry;

llu part1(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  Array *seeds = array_new(llu);
  Array *entries = array_new(AlmanachEntry);

  char source_type_str[BUFFER_LENGTH];
  enum AlmanachEntryType source_type = 0;

  char buffer[BUFFER_LENGTH] = {0};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    if (buffer[0] == '\0') {
      printf("reset state\n\n");
    } else if (strncmp(buffer, "seeds: ", 6) == 0) {
      char *seeds_str = substring(buffer, 7, buffer_len - 7 - 1);
      StringArray *split = string_split(seeds_str, " ");
      for (size_t i = 0; i < split->size; i++) {
        array_push(seeds, strtoull(split->items[i], NULL, 10));
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
      } else {
        printf("got unexpected source type %s\n", source_type_str);
        exit(42);
      }
      printf("new source type: \033[97;4m%s\033[0m\n", source_type_str);
    } else if ('0' <= buffer[0] && buffer[0] <= '9') {
      char *p;
      llu destination = strtoull(buffer, &p, 10);
      llu source = strtoull(p + 1, &p, 10);
      llu range = strtoull(p + 1, NULL, 10);
      printf("entry for source type \033[97;1m%s\033[0m: %llu %llu %llu\n", source_type_str, destination, source, range);

      AlmanachEntry entry = {
        .source_type = source_type,
        .destination = destination,
        .source = source,
        .range = range
      };
      array_push(entries, entry);
    } else {
      exit(42);
    }
  }
  printf("\n");

  llu min = ULLONG_MAX;
  for (size_t i = 0; i < seeds->size; i++) {
    llu seed_to_location = llu_array_get(seeds, i);
    for (size_t entry_type = 0; entry_type < NUMBER_OF_ALMANACH_ENTRY_TYPES; entry_type++) {
      bool stop = false;
      for (size_t j = 0; j < entries->size && !stop; j++) {
        AlmanachEntry *entry = array_get(entries, j);
        if (entry->source_type == entry_type) {
          if (entry->source <= seed_to_location && seed_to_location < entry->source + entry->range) {
            seed_to_location += entry->destination - entry->source;
            stop = true;
          }
        }
      }
    }
    if (seed_to_location < min) {
      min = seed_to_location;
    }
  }

  fclose(fp);

  printf("min = %llu\n", min);

  array_free(entries);
  array_free(seeds);

  return min;
}

llu part2(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  Array *pairs = array_new(llu);
  Array *entries = array_new(AlmanachEntry);

  char source_type_str[BUFFER_LENGTH];
  enum AlmanachEntryType source_type = 0;

  char buffer[BUFFER_LENGTH] = {0};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    if (buffer[0] == '\0') {
      printf("reset state\n\n");
    } else if (strncmp(buffer, "seeds: ", 6) == 0) {
      char *pairs_str = substring(buffer, 7, buffer_len - 7 - 1);
      StringArray *split = string_split(pairs_str, " ");
      for (size_t i = 0; i < split->size; i++) {
        array_push(pairs, strtoull(split->items[i], NULL, 0));
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
      } else {
        printf("got unexpected source type %s\n", source_type_str);
        exit(42);
      }
      printf("new source type: \033[97;4m%s\033[0m\n", source_type_str);
    } else if ('0' <= buffer[0] && buffer[0] <= '9') {
      char *p;
      llu destination = strtoull(buffer, &p, 10);
      llu source = strtoull(p + 1, &p, 10);
      llu range = strtoull(p + 1, NULL, 10);
      printf("entry for source type \033[97;1m%s\033[0m: %llu %llu %llu\n", source_type_str, destination, source, range);

      AlmanachEntry entry = {
        .source_type = source_type,
        .destination = destination,
        .source = source,
        .range = range
      };
      array_push(entries, entry);
    } else {
      exit(42);
    }
  }
  printf("\n");
  
  llu min = ULLONG_MAX;
  for (size_t i = 0; i < pairs->size; i += 2) {
    llu start = llu_array_get(pairs, i);
    llu end = start + llu_array_get(pairs, i + 1);
    for (llu seed = start; seed < end; seed++) {
      llu seed_to_location = seed;
      for (size_t entry_type = 0; entry_type < NUMBER_OF_ALMANACH_ENTRY_TYPES; entry_type++) {
        bool stop = false;
        for (size_t j = 0; j < entries->size && !stop; j++) {
          AlmanachEntry *entry = array_get(entries, j);
          if (entry->source_type == entry_type) {
            if (entry->source <= seed_to_location && seed_to_location < entry->source + entry->range) {
              seed_to_location += entry->destination - entry->source;
              stop = true;
            }
          }
        }
      }
      if (seed_to_location < min) {
        min = seed_to_location;
      }
    }
  }

  fclose(fp);

  printf("min = %llu\n", min);

  array_free(entries);
  array_free(pairs);

  return min;
}

int main() {
  assert(part1("../../inputs/2023/day5/sample") == 35);
  assert(part1("../../inputs/2023/day5/data") == 403695602);
  assert(part2("../../inputs/2023/day5/sample") == 46);
  assert(part2("../../inputs/2023/day5/data") == 219529182);

  return 0;
}
