#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string.h"

const unsigned int BUFFER_LENGTH = 256;

int part1(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  int result = 1;

  size_t times_size = 0;
  int times[10];

  size_t distances_size = 0;
  int distances[10];

  char buffer[BUFFER_LENGTH];
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    size_t j = 0;
    for (size_t i = 0; buffer[i] != '\0'; i++) {
      buffer[j] = buffer[i];
      if (buffer[i] != ' ' || (buffer[i] == ' ' && buffer[i + 1] != ' ')) {
        j++;
      }
    }
    buffer[j] = '\0';
    buffer_len = strlen(buffer);
    printf("buffer = [%c]|%s| (%zu)\n", buffer[0], buffer, j);

    if (buffer[0] == 'T') {
      char *colon_offset_p = strchr(buffer, ':');
      size_t colon_offset = colon_offset_p - buffer + 2;
      char *times_str = substring(buffer, colon_offset, buffer_len - colon_offset);
      printf("Time: %s\n", times_str);

      StringArray* times_split = string_split(times_str, " ");
      string_array_print(times_split);
      for (size_t i = 0; i < times_split->size; i++) {
        times[times_size++] = atoi(string_array_get(times_split, i));
      }

      free(times_str);
      string_array_free(times_split);
    } else if (buffer[0] == 'D') {
      char *colon_offset_p = strchr(buffer, ':');
      size_t colon_offset = colon_offset_p - buffer + 2;
      char *distances_str = substring(buffer, colon_offset, buffer_len - colon_offset);
      printf("Distance: %s\n", distances_str);

      StringArray* distances_split = string_split(distances_str, " ");
      string_array_print(distances_split);
      for (size_t i = 0; i < distances_split->size; i++) {
        distances[distances_size++] = atoi(string_array_get(distances_split, i));
      }

      free(distances_str);
      string_array_free(distances_split);
    }
    printf("\n");
  }

  if (times_size != distances_size) {
    exit(42);
  }

  for (size_t i = 0; i < times_size; i++) {
    int time = times[i];
    int distance = distances[i];
    printf("t = %d, d = %d\n", time, distance);

    int winning_count = 0;
    for (int hold = 0; hold <= time; hold++) {
      int d = 0;
      for (int time_left = 0; time_left < time - hold; time_left++) {
        d += hold;
      }

      printf("hold = %d, distance = %d", hold, d);
      if (d > distance) {
        winning_count++;
        printf(" (win)\n");
      } else {
        printf("\n");
      }
    }
    printf("wins = %d\n\n", winning_count);

    result *= winning_count;
  }

  fclose(fp);

  printf("result = %d\n", result);

  return result;
}

int part2(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  long long result = 1;

  size_t times_size = 0;
  long long int times[10];

  size_t distances_size = 0;
  long long distances[10];

  char buffer[BUFFER_LENGTH];
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    size_t j = 0;
    for (size_t i = 0; buffer[i] != '\0'; i++) {
      buffer[j] = buffer[i];
      if (buffer[i] != ' ') {
        j++;
      }
    }
    buffer[j] = '\0';
    buffer_len = strlen(buffer);
    printf("buffer = [%c]|%s| (%zu)\n", buffer[0], buffer, j);

    if (buffer[0] == 'T') {
      char *colon_offset_p = strchr(buffer, ':');
      size_t colon_offset = colon_offset_p - buffer + 1;
      char *times_str = substring(buffer, colon_offset, buffer_len - colon_offset);
      printf("Time: %s\n", times_str);

      StringArray* times_split = string_split(times_str, " ");
      string_array_print(times_split);
      for (size_t i = 0; i < times_split->size; i++) {
        times[times_size++] = atoll(string_array_get(times_split, i));
      }

      free(times_str);
      string_array_free(times_split);
    } else if (buffer[0] == 'D') {
      char *colon_offset_p = strchr(buffer, ':');
      size_t colon_offset = colon_offset_p - buffer + 1;
      char *distances_str = substring(buffer, colon_offset, buffer_len - colon_offset);
      printf("Distance: %s\n", distances_str);

      StringArray* distances_split = string_split(distances_str, " ");
      string_array_print(distances_split);
      for (size_t i = 0; i < distances_split->size; i++) {
        distances[distances_size++] = atoll(string_array_get(distances_split, i));
      }

      free(distances_str);
      string_array_free(distances_split);
    }
    printf("\n");
  }

  if (times_size != distances_size) {
    exit(42);
  }

  for (size_t i = 0; i < times_size; i++) {
    long long time = times[i];
    long long distance = distances[i];
    //printf("t = %d, d = %d\n", time, distance);

    long long winning_count = 0;
    for (int hold = 0; hold <= time; hold++) {
      long long d = 0;
      for (long long time_left = 0; time_left < time - hold; time_left++) {
        d += hold;
      }

      //printf("hold = %lld, distance = %lld", hold, d);
      if (d > distance) {
        winning_count++;
        //printf(" (win)\n");
      } else {
        //printf("\n");
      }
    }
    //printf("wins = %lld\n\n", winning_count);

    result *= winning_count;
  }

  fclose(fp);

  printf("result = %d\n", result);

  return result;
}

int main() {
  //assert(part1("../../inputs/2023/day6/part1_test") == 288);
  //assert(part1("../../inputs/2023/day6/data") == 449550);
  //assert(part2("../../inputs/2023/day6/part2_test") == 71503);
  assert(part2("../../inputs/2023/day6/data") == 28360140);

  return 0;
}
