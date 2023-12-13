#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "string.h"

#define BUFFER_LENGTH 1024

int part1(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  int result = 1;

  Array *times = array_new(int);
  Array *distances = array_new(int);

  char buffer[BUFFER_LENGTH] = {0};
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
        array_push(times, atoi(string_array_get(times_split, i)));
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
        array_push(distances, atoi(string_array_get(distances_split, i)));
      }

      free(distances_str);
      string_array_free(distances_split);
    }
    printf("\n");
  }

  if (times->size != distances->size) {
    exit(42);
  }

  for (size_t i = 0; i < times->size; i++) {
    int time = int_array_get(times, i);
    int distance = int_array_get(distances, i);
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

  array_free(times);
  array_free(distances);

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

  Array *times = array_new(long long);
  Array *distances = array_new(long long);

  char buffer[BUFFER_LENGTH] = {0};
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
        array_push(times, atoll(string_array_get(times_split, i)));
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
        array_push(distances, atoll(string_array_get(distances_split, i)));
      }

      free(distances_str);
      string_array_free(distances_split);
    }
    printf("\n");
  }

  if (times->size != distances->size) {
    exit(42);
  }

  for (size_t i = 0; i < times->size; i++) {
    long long time = ll_array_get(times, i);
    long long distance = ll_array_get(distances, i);
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

  array_free(times);
  array_free(distances);

  printf("result = %llu\n", result);

  return result;
}

int main() {
  assert(part1("../../inputs/2023/day6/part1_test") == 288);
  assert(part1("../../inputs/2023/day6/data") == 449550);
  assert(part2("../../inputs/2023/day6/part2_test") == 71503);
  assert(part2("../../inputs/2023/day6/data") == 28360140);

  return 0;
}
