#include <assert.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_LENGTH 1024

int part1(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  int reds = 12;
  int greens = 13;
  int blues = 14;

  int sum = 0;
  int current_game = 0;

  regex_t regex;
  size_t groups = 3; // ignoring the last group
  regmatch_t match[groups];
  char *pattern = " *([0-9]+) (red|green|blue)( |\n)*";
  if (regcomp(&regex, pattern, REG_EXTENDED)) {
    fprintf(stderr, "Error: could not compile regex `%s`\n", pattern);
    return 1;
  }

  char buffer[BUFFER_LENGTH] = {0};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    char *p_token_save;
    char *token = strtok_r(buffer, ":", &p_token_save);
    while (token != NULL) {
      if (token[0] == 'G') { // game
        current_game += 1;
      } else { // pairs
        bool valid = true;
        char *p_token2_save;
        char *token2 = strtok_r(token, ";", &p_token2_save);
        while (token2 != NULL) { // pairs
          char *p_token3_save;
          char *token3 = strtok_r(token2, ",", &p_token3_save);
          while (token3 != NULL) { // pair
            int reti = regexec(&regex, token3, groups, match, REG_EXTENDED);
            int n;
            if (!reti) {
              for (size_t g = 1; g < groups; g++) {
                regoff_t start = match[g].rm_so;
                if (start == -1) {
                  break;
                }
                regoff_t end = match[g].rm_eo;
                char copy[end - start + 1];
                strncpy(copy, token3 + start, end - start);
                copy[end - start] = 0;
                if (g == 1) {
                  n = atoi(copy);
                } else if (copy[0] == 'g' && n > greens) {
                  valid = false;
                } else if (copy[0] == 'r' && n > reds) {
                  valid = false;
                } else if (copy[0] == 'b' && n > blues) {
                  valid = false;
                }
              }
            } else if (reti == REG_NOMATCH) {
              fprintf(stderr, "Error: no match for `%s`", token3);
              return 1;
            } else {
              fprintf(stderr, "Error: regex match failed for `%s`", token3);
              return 1;
            }
            token3 = strtok_r(NULL, ",", &p_token3_save);
          }
          token2 = strtok_r(NULL, ";", &p_token2_save);
        }
        if (valid) {
          sum += current_game;
        }
      }
      token = strtok_r(NULL, ":", &p_token_save);
    }
  }
  
  fclose(fp);
  regfree(&regex);

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

  regex_t regex;
  size_t groups = 3; // ignoring the last group
  regmatch_t match[groups];
  char *pattern = " *([0-9]+) (red|green|blue)( |\n)*";
  if (regcomp(&regex, pattern, REG_EXTENDED)) {
    fprintf(stderr, "Error: could not compile regex `%s`\n", pattern);
    return 1;
  }

  char buffer[BUFFER_LENGTH] = {0};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    char *p_token_save;
    char *token = strtok_r(buffer, ":", &p_token_save);
    while (token != NULL) {
      if (token[0] != 'G') { // game
        int reds = -1;
        int greens = -1;
        int blues = -1;

        char *p_token2_save;
        char *token2 = strtok_r(token, ";", &p_token2_save);
        while (token2 != NULL) { // pairs
          char *p_token3_save;
          char *token3 = strtok_r(token2, ",", &p_token3_save);
          while (token3 != NULL) { // pair
            int reti = regexec(&regex, token3, groups, match, REG_EXTENDED);
            int n;
            if (!reti) {
              for (size_t g = 1; g < groups; g++) {
                regoff_t start = match[g].rm_so;
                if (start == -1) {
                  break;
                }
                regoff_t end = match[g].rm_eo;
                char copy[end - start + 1];
                strncpy(copy, token3 + start, end - start);
                copy[end - start] = 0;
                if (g == 1) {
                  n = atoi(copy);
                } else if (copy[0] == 'r' && n > reds) {
                  reds = n;
                } else if (copy[0] == 'g' && n > greens) {
                  greens = n;
                } else if (copy[0] == 'b' && n > blues) {
                  blues = n;
                }
              }
            } else if (reti == REG_NOMATCH) {
              fprintf(stderr, "Error: no match for `%s`", token3);
              return 1;
            } else {
              fprintf(stderr, "Error: regex match failed for `%s`", token3);
              return 1;
            }
            token3 = strtok_r(NULL, ",", &p_token3_save);
          }
          token2 = strtok_r(NULL, ";", &p_token2_save);
        }
        int power = reds * greens * blues;
        printf("(%d, %d, %d) = %d\n", reds, greens, blues, power);
        sum += power;
      }
      token = strtok_r(NULL, ":", &p_token_save);
    }
  }

  fclose(fp);
  regfree(&regex);

  printf("sum: %d\n", sum);
  
  return sum;
}

int main(void) {
  assert(part1("../../inputs/2023/day2/sample") == 8);
  assert(part1("../../inputs/2023/day2/data") == 2913);
  assert(part2("../../inputs/2023/day2/sample") == 2286);
  assert(part2("../../inputs/2023/day2/data") == 55593);

  return 0;
}
