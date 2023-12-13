#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "string.h"

#define BUFFER_LENGTH 1024

Array *get_numbers(char *str) {
  Array *numbers = array_new(int);
  StringArray *split = string_split(str, " ");
  for (size_t i = 0; i < split->size; i++) {
    int number = atoi(string_array_get(split, i));
    if (number > 0) {
      array_push(numbers, number);
    }
  }

  string_array_free(split);

  return numbers;
}

int part1(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  int card = 1;
  int sum = 0;

  char buffer[BUFFER_LENGTH] = {0};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    size_t colon_offset = strchr(buffer, ':') - buffer;
    size_t pipe_offset = strchr(buffer, '|') - buffer;
    size_t winning_numbers_len = (pipe_offset - 1) - (colon_offset + 2);
    size_t numbers_len = buffer_len - (pipe_offset + 2);
  
    //           11111111112222222222333333333344444444
    // 012345678901234567890123456789012345678901234567
    // Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
    //       ^ colon_offset   ^ pipe_offset
    char *winning_numbers_buf = substring(buffer, colon_offset + 2, winning_numbers_len);
    char *numbers_buf = substring(buffer, pipe_offset + 2, numbers_len);

    //printf("winning numbers buffer: |%s|\n", winning_numbers_buf);
    //printf("numbers buffer: |%s|\n", numbers_buf);

    Array *winning_numbers = get_numbers(winning_numbers_buf);
    //int_array_print(winning_numbers);
    Array *numbers = get_numbers(numbers_buf);
    //int_array_print(numbers);

    int points = 0;
    for (size_t i = 0; i < winning_numbers->size; i++) {
      int winning_number = int_array_get(winning_numbers, i);
      //printf("%d? %s\n", winning_number, array_contains(numbers, winning_number) ? "true" : "false");
      if (int_array_contains(numbers, winning_number)) {
        points = points == 0 ? 1 : points * 2;
        //printf("%d\n", points);
      }
    }

    printf("Card %d is worth %d points\n", card++, points);
    sum += points;

    array_free(numbers);
    array_free(winning_numbers);
    free(numbers_buf);
    free(winning_numbers_buf);
  }
  
  fclose(fp);

  printf("sum = %d\n", sum);

  return sum;
}

int array_sum(int *array, size_t size) {
  int sum = 0;
  for (size_t i = 0; i < size; i++) {
    sum += array[i];
  }
  return sum;
}

int part2(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  int card = 1;
  int cards[1000] = {0};
  cards[card - 1] = 0;

  char buffer[BUFFER_LENGTH] = {0};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    cards[card - 1] += 1;

    size_t colon_offset = strchr(buffer, ':') - buffer;
    size_t pipe_offset = strchr(buffer, '|') - buffer;
    size_t winning_numbers_len = (pipe_offset - 1) - (colon_offset + 2);
    size_t numbers_len = buffer_len - (pipe_offset + 2);
  
    //           11111111112222222222333333333344444444
    // 012345678901234567890123456789012345678901234567
    // Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
    //       ^ colon_offset   ^ pipe_offset
    char *winning_numbers_buf = substring(buffer, colon_offset + 2, winning_numbers_len);
    char *numbers_buf = substring(buffer, pipe_offset + 2, numbers_len);

    //printf("winning numbers buffer: |%s|\n", winning_numbers_buf);
    //printf("numbers buffer: |%s|\n", numbers_buf);

    Array *winning_numbers = get_numbers(winning_numbers_buf);
    //int_array_print(winning_numbers);
    Array *numbers = get_numbers(numbers_buf);
    //int_array_print(numbers);

    int points = 0;
    for (size_t i = 0; i < winning_numbers->size; i++) {
      int winning_number = int_array_get(winning_numbers, i);
      //printf("%d? %s\n", winning_number, array_contains(numbers, winning_number) ? "true" : "false");
      if (int_array_contains(numbers, winning_number)) {
        points++;
        //printf("%d\n", points);
      }
    }

    int iterations = cards[card - 1];
    printf("Game %d adds %d times to the next %d cards\n", card, iterations, points);
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

  int sum = array_sum(cards, card - 1);
  printf("sum = %d\n", sum);

  return sum;
}

int main() {
  assert(part1("../../inputs/2023/day4/part1_test") == 13);
  assert(part1("../../inputs/2023/day4/data") == 21088);
  assert(part2("../../inputs/2023/day4/part2_test") == 30);
  assert(part2("../../inputs/2023/day4/data") == 6874754);

  return 0;
}
