#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"

#define BUFFER_LENGTH 1024

int part1(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  int sum = 0;

  char buffer[BUFFER_LENGTH] = {0};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    Array *digits = array_new(int);
    for (int i = 0; buffer[i] != '\n'; i++) {
      int value = buffer[i] - '0';
      if (value > 0 && value < 10) {
        array_push(digits, value);
      }
    }
    sum += int_array_first(digits) * 10 + int_array_last(digits);
    array_free(digits);
  }
  
  fclose(fp);

  printf("sum: %d\n", sum);

  return sum;
}

int part2(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s", filename);
    return 1;
  }

  int sum = 0;

  char buffer[BUFFER_LENGTH] = {0};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    Array *digits = array_new(int);
    size_t len = strlen(buffer) - 1; // \n counts but not \0
    for (size_t i = 0; buffer[i] != '\n'; i++) {
      char c = buffer[i];
      if (c >= '1' && c <= '9') {
        array_push(digits, c - '0');
      } else {
        // one two three four five six seven eight nine
        // o   t         f         s         e     n
        if (c == 'o' && (i+2) < len && buffer[i+1] == 'n' && buffer[i+2] == 'e') array_push(digits, 1);
        if (c == 't' && (i+2) < len && buffer[i+1] == 'w' && buffer[i+2] == 'o') array_push(digits, 2);
        if (c == 't' && (i+4) < len && buffer[i+1] == 'h' && buffer[i+2] == 'r' && buffer[i+3] == 'e' && buffer[i+4] == 'e') array_push(digits, 3);
        if (c == 'f' && (i+3) < len && buffer[i+1] == 'o' && buffer[i+2] == 'u' && buffer[i+3] == 'r') array_push(digits, 4);
        if (c == 'f' && (i+3) < len && buffer[i+1] == 'i' && buffer[i+2] == 'v' && buffer[i+3] == 'e') array_push(digits, 5);
        if (c == 's' && (i+2) < len && buffer[i+1] == 'i' && buffer[i+2] == 'x') array_push(digits, 6);
        if (c == 's' && (i+4) < len && buffer[i+1] == 'e' && buffer[i+2] == 'v' && buffer[i+3] == 'e' && buffer[i+4] == 'n') array_push(digits, 7);
        if (c == 'e' && (i+4) < len && buffer[i+1] == 'i' && buffer[i+2] == 'g' && buffer[i+3] == 'h' && buffer[i+4] == 't') array_push(digits, 8);
        if (c == 'n' && (i+3) < len && buffer[i+1] == 'i' && buffer[i+2] == 'n' && buffer[i+3] == 'e') array_push(digits, 9);
      }
    }
    sum += int_array_first(digits) * 10 + int_array_last(digits);
    int_array_print(digits);
    array_free(digits);
  }
  
  fclose(fp);

  printf("sum: %d\n", sum);

  return sum;
}

int main() {
  assert(part1("../../inputs/2023/day1/part1_test") == 142);
  assert(part1("../../inputs/2023/day1/data") == 55488);
  assert(part2("../../inputs/2023/day1/part2_test") == 281);
  assert(part2("../../inputs/2023/day1/data") == 55614);

  return 0;
}
