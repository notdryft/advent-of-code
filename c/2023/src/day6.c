#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "string.h"

int part1(StringArray *lines) {
  int result = 1;

  Array *times = array_new(int);
  Array *distances = array_new(int);

  for (size_t l = 0; l < lines->size; l++) {
    char *line = lines->items[l];
    size_t line_len = strlen(line);

    size_t j = 0;
    for (size_t i = 0; i < line_len; i++) {
      line[j] = line[i];
      if (line[i] != ' ' || (line[i] == ' ' && line[i + 1] != ' ')) {
        j++;
      }
    }
    line[j] = '\0';
    line_len = strlen(line);
    debug("line = [%c]|%s| (%zu)\n", line[0], line, j);

    if (line[0] == 'T') {
      char *colon_offset_p = strchr(line, ':');
      size_t colon_offset = colon_offset_p - line + 2;
      char *times_str = substring(line, colon_offset, line_len - colon_offset);
      debug("Time: %s\n", times_str);

      StringArray* times_split = string_split(times_str, " ");
      debugf(string_array_print, times_split);
      for (size_t i = 0; i < times_split->size; i++) {
        array_push_rval(times, atoi(string_array_get(times_split, i)));
      }

      free(times_str);
      string_array_free(times_split);
    } else if (line[0] == 'D') {
      char *colon_offset_p = strchr(line, ':');
      size_t colon_offset = colon_offset_p - line + 2;
      char *distances_str = substring(line, colon_offset, line_len - colon_offset);
      debug("Distance: %s\n", distances_str);

      StringArray* distances_split = string_split(distances_str, " ");
      debugf(string_array_print, distances_split);
      for (size_t i = 0; i < distances_split->size; i++) {
        array_push_rval(distances, atoi(string_array_get(distances_split, i)));
      }

      free(distances_str);
      string_array_free(distances_split);
    }
    debug("\n");
  }

  if (times->size != distances->size) {
    exit(42);
  }

  for (size_t i = 0; i < times->size; i++) {
    int time = int_array_get(times, i);
    int distance = int_array_get(distances, i);
    debug("t = %d, d = %d\n", time, distance);

    int winning_count = 0;
    for (int hold = 0; hold <= time; hold++) {
      int d = 0;
      for (int time_left = 0; time_left < time - hold; time_left++) {
        d += hold;
      }

      debug("hold = %d, distance = %d", hold, d);
      if (d > distance) {
        winning_count++;
        debug(" (win)\n");
      } else {
        debug("\n");
      }
    }
    debug("wins = %d\n\n", winning_count);

    result *= winning_count;
  }

  array_free(times);
  array_free(distances);

  return result;
}

long long part2(StringArray *lines) {
  long long result = 1;

  Array *times = array_new(long long);
  Array *distances = array_new(long long);

  for (size_t l = 0; l < lines->size; l++) {
    char *line = lines->items[l];
    size_t line_len = strlen(line);

    size_t j = 0;
    for (size_t i = 0; i < line_len; i++) {
      line[j] = line[i];
      if (line[i] != ' ') {
        j++;
      }
    }
    line[j] = '\0';
    line_len = strlen(line);
    debug("line = [%c]|%s| (%zu)\n", line[0], line, j);

    if (line[0] == 'T') {
      char *colon_offset_p = strchr(line, ':');
      size_t colon_offset = colon_offset_p - line + 1;
      char *times_str = substring(line, colon_offset, line_len - colon_offset);
      debug("Time: %s\n", times_str);

      StringArray* times_split = string_split(times_str, " ");
      debugf(string_array_print, times_split);
      for (size_t i = 0; i < times_split->size; i++) {
        array_push_rval(times, atoll(string_array_get(times_split, i)));
      }

      free(times_str);
      string_array_free(times_split);
    } else if (line[0] == 'D') {
      char *colon_offset_p = strchr(line, ':');
      size_t colon_offset = colon_offset_p - line + 1;
      char *distances_str = substring(line, colon_offset, line_len - colon_offset);
      debug("Distance: %s\n", distances_str);

      StringArray* distances_split = string_split(distances_str, " ");
      debugf(string_array_print, distances_split);
      for (size_t i = 0; i < distances_split->size; i++) {
        array_push_rval(distances, atoll(string_array_get(distances_split, i)));
      }

      free(distances_str);
      string_array_free(distances_split);
    }
    debug("\n");
  }

  if (times->size != distances->size) {
    exit(42);
  }

  for (size_t i = 0; i < times->size; i++) {
    long long time = ll_array_get(times, i);
    long long distance = ll_array_get(distances, i);
    debug("t = %lld, d = %lld\n", time, distance);

    long long winning_count = 0;
    for (long long hold = 0; hold <= time; hold++) {
      long long d = 0;
      for (long long time_left = 0; time_left < time - hold; time_left++) {
        d += hold;
      }

      //debug("hold = %lld, distance = %lld", hold, d);
      if (d > distance) {
        winning_count++;
        //debug(" (win)\n");
      } else {
        //debug("\n");
      }
    }
    debug("wins = %lld\n\n", winning_count);

    result *= winning_count;
  }

  array_free(times);
  array_free(distances);

  return result;
}

int main(void) {
  test_case(day6, part1, sample, 288);
  test_case(day6, part1, data, 449550);
  test_case(day6, part2, sample, 71503);
  test_case(day6, part2, data, 28360140);

  return 0;
}
