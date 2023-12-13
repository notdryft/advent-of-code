#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string.h"

#define BUFFER_LENGTH 1024

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
  long long destination;
  long long source;
  long long range;
} AlmanachEntry;

int part1(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  size_t capacity = 1000;

  size_t seeds_size = 0;
  long long *seeds = (long long *) malloc(sizeof(long long *) * capacity);

  size_t map_size = 0;
  AlmanachEntry *map = (AlmanachEntry *) malloc(sizeof(AlmanachEntry *) * capacity);

  char source_type_str[BUFFER_LENGTH];
  enum AlmanachEntryType source_type = 0;

  char source_str[BUFFER_LENGTH];
  char destination_str[BUFFER_LENGTH];
  char range_str[BUFFER_LENGTH];

  char buffer[BUFFER_LENGTH] = {0};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    printf("parsing: |%s| with first char '%c'\n", buffer, buffer[0]);
    if (buffer[0] == '\0') { // reset state
      printf("reset\n\n");
    } else if (strncmp(buffer, "seeds: ", 6) == 0) {
      char *seeds_str = substring(buffer, 7, buffer_len - 7 - 1);
      StringArray *seeds_split = string_split(seeds_str, " ");
      for (size_t i = 0; i < seeds_split->size; i++) {
        seeds[seeds_size++] = atoll(string_array_get(seeds_split, i));
      }

      free(seeds_str);
      string_array_free(seeds_split);
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
      printf("new source type: <%s> to <?> \n", source_type_str);
    } else if (
      '0' <= buffer[0] && buffer[0] <= '9' && 
      sscanf(buffer, "%s %s %s", destination_str, source_str, range_str)
    ) {
      long long destination = atoll(destination_str);
      long long source = atoll(source_str);
      long long range = atoll(range_str);
      printf("entry for source type <%s>: %lld %lld %lld\n", source_type_str, destination, source, range);

      AlmanachEntry entry = {
        .source_type = source_type,
        .destination = destination,
        .source = source,
        .range = range
      };
      map[map_size++] = entry;
    } else {
      exit(42);
    }
  }
  
  printf("\n\\\\\n\n");

  long long location_min = LLONG_MAX;
  for (size_t seed_index = 0; seed_index < seeds_size; seed_index++) {
    long long seed_to_location = seeds[seed_index];
    for (size_t entry_type = 0; entry_type < NUMBER_OF_ALMANACH_ENTRY_TYPES; entry_type++) {
      bool found_entry = false;
      for (size_t entry_index = 0; entry_index < map_size && !found_entry; entry_index++) {
        AlmanachEntry entry = map[entry_index];
        if (entry.source_type == entry_type) {
          if (entry.source <= seed_to_location && seed_to_location < entry.source + entry.range) {
            found_entry = true;
            seed_to_location += entry.destination - entry.source;
          }
        }
      }
    }
    if (seed_to_location < location_min) {
      location_min = seed_to_location;
    }
  }

  fclose(fp);
  free(seeds);
  free(map);

  printf("location min = %lld\n", location_min);

  return location_min;
}

int part2(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  size_t capacity = 1000;

  size_t pairs_size = 0;
  long long *pairs = (long long *) malloc(sizeof(long long *) * capacity);

  size_t map_size = 0;
  AlmanachEntry *map = (AlmanachEntry *) malloc(sizeof(AlmanachEntry *) * capacity);

  char source_type_str[BUFFER_LENGTH];
  enum AlmanachEntryType source_type = 0;

  char source_str[BUFFER_LENGTH];
  char destination_str[BUFFER_LENGTH];
  char range_str[BUFFER_LENGTH];

  char buffer[BUFFER_LENGTH] = {0};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    printf("parsing: |%s| with first char '%c'\n", buffer, buffer[0]);
    if (buffer[0] == '\0') { // reset state
      printf("reset\n\n");
    } else if (strncmp(buffer, "seeds: ", 6) == 0) {
      char *pairs_str = substring(buffer, 7, buffer_len - 7 - 1);
      StringArray *pairs_split = string_split(pairs_str, " ");
      for (size_t i = 0; i < pairs_split->size; i++) {
        pairs[pairs_size++] = atoll(string_array_get(pairs_split, i));
      }

      free(pairs_str);
      string_array_free(pairs_split);
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
      printf("new source type: <%s> to <?> \n", source_type_str);
    } else if (
      '0' <= buffer[0] && buffer[0] <= '9' && 
      sscanf(buffer, "%s %s %s", destination_str, source_str, range_str)
    ) {
      long long destination = atoll(destination_str);
      long long source = atoll(source_str);
      long long range = atoll(range_str);
      printf("entry for source type <%s>: %lld %lld %lld\n", source_type_str, destination, source, range);

      AlmanachEntry entry = {
        .source_type = source_type,
        .destination = destination,
        .source = source,
        .range = range
      };
      map[map_size++] = entry;
    } else {
      exit(42);
    }
  }
  
  printf("\n\\\\\n\n");

  long long location_min = LLONG_MAX;
  for (size_t pair_index = 0; pair_index < pairs_size; pair_index += 2) {
    long long seed_start = pairs[pair_index];
    long long seed_end = seed_start + pairs[pair_index + 1];
    for (long long seed = seed_start; seed < seed_end; seed++) {
      long long seed_to_location = seed;
      for (size_t entry_type = 0; entry_type < NUMBER_OF_ALMANACH_ENTRY_TYPES; entry_type++) {
        bool found_entry = false;
        for (size_t entry_index = 0; entry_index < map_size && !found_entry; entry_index++) {
          AlmanachEntry entry = map[entry_index];
          if (entry.source_type == entry_type) {
            if (entry.source <= seed_to_location && seed_to_location < entry.source + entry.range) {
              found_entry = true;
              seed_to_location += entry.destination - entry.source;
            }
          }
        }
      }
      if (seed_to_location < location_min) {
        location_min = seed_to_location;
      }
    }
  }

  fclose(fp);
  free(pairs);
  free(map);

  printf("location min = %llu\n", location_min);

  return location_min;
}

int main() {
  assert(part1("../../inputs/2023/day5/part1_test") == 35);
  assert(part1("../../inputs/2023/day5/data") == 403695602);
  assert(part2("../../inputs/2023/day5/part2_test") == 46);
  assert(part2("../../inputs/2023/day5/data") == 219529182);

  return 0;
}
