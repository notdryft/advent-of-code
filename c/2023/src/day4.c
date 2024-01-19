#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "string.h"

Array *get_numbers(char *str) {
  Array *numbers = array_new(int);
  StringArray *split = string_split(str, " ");
  for (size_t i = 0; i < split->size; i++) {
    int number = atoi(string_array_get(split, i));
    if (number > 0) {
      array_push_rval(numbers, number);
    }
  }

  string_array_free(split);

  return numbers;
}

int part1(StringArray *lines) {
  int card = 1;
  int result = 0;

  string_array_foreach (char *line, lines) {
    size_t line_len = strlen(line);

    size_t colon_offset = strchr(line, ':') - line;
    size_t pipe_offset = strchr(line, '|') - line;
    size_t winning_numbers_len = (pipe_offset - 1) - (colon_offset + 2);
    size_t numbers_len = line_len - (pipe_offset + 2);

    //           11111111112222222222333333333344444444
    // 012345678901234567890123456789012345678901234567
    // Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
    //       ^ colon_offset   ^ pipe_offset
    char *winning_numbers_buf = substring(line, colon_offset + 2, winning_numbers_len);
    char *numbers_buf = substring(line, pipe_offset + 2, numbers_len);

    debug("winning numbers buffer: |%s|\n", winning_numbers_buf);
    debug("numbers buffer: |%s|\n", numbers_buf);

    Array *winning_numbers = get_numbers(winning_numbers_buf);
    debugf(int_array_print, winning_numbers);
    Array *numbers = get_numbers(numbers_buf);
    debugf(int_array_print, numbers);

    int points = 0;
    for (size_t i = 0; i < winning_numbers->size; i++) {
      int winning_number = int_array_get(winning_numbers, i);
      debug("%d? %s\n", winning_number, int_array_contains(numbers, winning_number) ? "true" : "false");
      if (int_array_contains(numbers, winning_number)) {
        points = points == 0 ? 1 : points * 2;
        debug("%d\n", points);
      }
    }

    card++;
    debug("Card %d is worth %d points\n", card, points);
    result += points;

    array_free(numbers);
    array_free(winning_numbers);
    free(numbers_buf);
    free(winning_numbers_buf);
  }

  return result;
}

int array_sum(int *array, size_t size) {
  int sum = 0;
  for (size_t i = 0; i < size; i++) {
    sum += array[i];
  }
  return sum;
}

int part2(StringArray *lines) {
  int card = 1;
  int cards[1000] = {};
  cards[card - 1] = 0;

  string_array_foreach (char *line, lines) {
    size_t line_len = strlen(line);

    cards[card - 1] += 1;

    size_t colon_offset = strchr(line, ':') - line;
    size_t pipe_offset = strchr(line, '|') - line;
    size_t winning_numbers_len = (pipe_offset - 1) - (colon_offset + 2);
    size_t numbers_len = line_len - (pipe_offset + 2);

    //           11111111112222222222333333333344444444
    // 012345678901234567890123456789012345678901234567
    // Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
    //       ^ colon_offset   ^ pipe_offset
    char *winning_numbers_buf = substring(line, colon_offset + 2, winning_numbers_len);
    char *numbers_buf = substring(line, pipe_offset + 2, numbers_len);

    debug("winning numbers buffer: |%s|\n", winning_numbers_buf);
    debug("numbers buffer: |%s|\n", numbers_buf);

    Array *winning_numbers = get_numbers(winning_numbers_buf);
    debugf(int_array_print, winning_numbers);
    Array *numbers = get_numbers(numbers_buf);
    debugf(int_array_print, numbers);

    int points = 0;
    for (size_t i = 0; i < winning_numbers->size; i++) {
      int winning_number = int_array_get(winning_numbers, i);
      debug("%d? %s\n", winning_number, int_array_contains(numbers, winning_number) ? "true" : "false");
      if (int_array_contains(numbers, winning_number)) {
        points++;
        debug("%d\n", points);
      }
    }

    int iterations = cards[card - 1];
    debug("Game %d adds %d times to the next %d cards\n", card, iterations, points);
    if (points > 0) {
      for (int i = card; i < card + points; i++) {
        cards[i] += iterations;
      }
    }
    card++;

    array_free(numbers);
    array_free(winning_numbers);
    free(numbers_buf);
    free(winning_numbers_buf);
  }

  return array_sum(cards, card - 1);
}

int main(void) {
  test_case(day4, part1, sample, 13);
  test_case(day4, part1, data, 21088);
  test_case(day4, part2, sample, 30);
  test_case(day4, part2, data, 6874754);

  return 0;
}
