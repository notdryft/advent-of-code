#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"

const unsigned int MAX_LENGTH = 256;

int part1(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Error: could not open file %s", filename);
    return 1;
  }

  int sum = 0;

  char buffer[MAX_LENGTH];
  while (fgets(buffer, MAX_LENGTH, fp)) {
    Array *array = array_new();
    for (int i = 0; buffer[i] != '\n'; i++) {
      int value = buffer[i] - '0';
      if (value > 0 && value < 10) {
        array_push(array, value);
      }
    }
    sum += array_first(array) * 10 + array_last(array);
    array_free(array);
  }

  printf("sum: %d\n", sum);
  
  fclose(fp);

  return 0;
}

int part2(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Error: could not open file %s", filename);
    return 1;
  }

  int sum = 0;

  char buffer[MAX_LENGTH];
  while (fgets(buffer, MAX_LENGTH, fp)) {
    Array *array = array_new();
    size_t len = strlen(buffer) - 1; // /n counts but not \0
    for (size_t i = 0; buffer[i] != '\n'; i++) {
      char c = buffer[i];
      if (c >= '1' && c <= '9') {
        array_push(array, c - '0');
      } else {
        // one two three four five six seven eight nine
        // o   t         f         s         e     n
        if (c == 'o' && (i+2) < len && buffer[i+1] == 'n' && buffer[i+2] == 'e') array_push(array, 1);
        if (c == 't' && (i+2) < len && buffer[i+1] == 'w' && buffer[i+2] == 'o') array_push(array, 2);
        if (c == 't' && (i+4) < len && buffer[i+1] == 'h' && buffer[i+2] == 'r' && buffer[i+3] == 'e' && buffer[i+4] == 'e') array_push(array, 3);
        if (c == 'f' && (i+3) < len && buffer[i+1] == 'o' && buffer[i+2] == 'u' && buffer[i+3] == 'r') array_push(array, 4);
        if (c == 'f' && (i+3) < len && buffer[i+1] == 'i' && buffer[i+2] == 'v' && buffer[i+3] == 'e') array_push(array, 5);
        if (c == 's' && (i+2) < len && buffer[i+1] == 'i' && buffer[i+2] == 'x') array_push(array, 6);
        if (c == 's' && (i+4) < len && buffer[i+1] == 'e' && buffer[i+2] == 'v' && buffer[i+3] == 'e' && buffer[i+4] == 'n') array_push(array, 7);
        if (c == 'e' && (i+4) < len && buffer[i+1] == 'i' && buffer[i+2] == 'g' && buffer[i+3] == 'h' && buffer[i+4] == 't') array_push(array, 8);
        if (c == 'n' && (i+3) < len && buffer[i+1] == 'i' && buffer[i+2] == 'n' && buffer[i+3] == 'e') array_push(array, 9);
      }
    }
    sum += array_first(array) * 10 + array_last(array);
    array_print(array);
    array_free(array);
  }

  printf("sum: %d\n", sum);
  
  fclose(fp);

  return 0;
}

int main() {
  //return part1("../../inputs/2023/day1/part1_test");
  //return part1("../../inputs/2023/day1/data");
  //return part2("../../inputs/2023/day1/part2_test");
  return part2("../../inputs/2023/day1/data");
}
