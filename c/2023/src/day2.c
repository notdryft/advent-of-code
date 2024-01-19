#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commons.h"

constexpr size_t GROUPS = 3; // ignoring the last group

int part1(StringArray *lines) {
  int reds = 12;
  int greens = 13;
  int blues = 14;

  int result = 0;
  int current_game = 0;

  regex_t regex;
  regmatch_t match[GROUPS];
  char *pattern = " *([0-9]+) (red|green|blue)( |\n)*";
  if (regcomp(&regex, pattern, REG_EXTENDED)) {
    fprintf(stderr, "Error: could not compile regex `%s`\n", pattern);
    return 1;
  }

  string_array_foreach (char *line, lines) {
    char *p_token_save;
    char *token = strtok_r(line, ":", &p_token_save);
    while (token != nullptr) {
      if (token[0] == 'G') { // game
        current_game += 1;
      } else { // pairs
        bool valid = true;
        char *p_token2_save;
        char *token2 = strtok_r(token, ";", &p_token2_save);
        while (token2 != nullptr) { // pairs
          char *p_token3_save;
          char *token3 = strtok_r(token2, ",", &p_token3_save);
          while (token3 != nullptr) { // pair
            int reti = regexec(&regex, token3, GROUPS, match, REG_EXTENDED);
            int n;
            if (!reti) {
              for (size_t g = 1; g < GROUPS; g++) {
                regoff_t start = match[g].rm_so;
                if (start == -1) {
                  break;
                }
                regoff_t end = match[g].rm_eo;
                char *copy = calloc(end - start + 1, sizeof(*copy));
                strncpy(copy, token3 + start, end - start);
                if (g == 1) {
                  n = atoi(copy);
                } else if (copy[0] == 'g' && n > greens) {
                  valid = false;
                } else if (copy[0] == 'r' && n > reds) {
                  valid = false;
                } else if (copy[0] == 'b' && n > blues) {
                  valid = false;
                }
                free(copy);
              }
            } else if (reti == REG_NOMATCH) {
              fprintf(stderr, "Error: no match for `%s`", token3);
              return 1;
            } else {
              fprintf(stderr, "Error: regex match failed for `%s`", token3);
              return 1;
            }
            token3 = strtok_r(nullptr, ",", &p_token3_save);
          }
          token2 = strtok_r(nullptr, ";", &p_token2_save);
        }
        if (valid) {
          result += current_game;
        }
      }
      token = strtok_r(nullptr, ":", &p_token_save);
    }
  }
  regfree(&regex);

  return result;
}

int part2(StringArray *lines) {
  int result = 0;

  regex_t regex;
  regmatch_t match[GROUPS];
  char *pattern = " *([0-9]+) (red|green|blue)( |\n)*";
  if (regcomp(&regex, pattern, REG_EXTENDED)) {
    fprintf(stderr, "Error: could not compile regex `%s`\n", pattern);
    return 1;
  }

  string_array_foreach(char *line, lines) {
    char *p_token_save;
    char *token = strtok_r(line, ":", &p_token_save);
    while (token != nullptr) {
      if (token[0] != 'G') { // game
        int reds = -1;
        int greens = -1;
        int blues = -1;

        char *p_token2_save;
        char *token2 = strtok_r(token, ";", &p_token2_save);
        while (token2 != nullptr) { // pairs
          char *p_token3_save;
          char *token3 = strtok_r(token2, ",", &p_token3_save);
          while (token3 != nullptr) { // pair
            int reti = regexec(&regex, token3, GROUPS, match, REG_EXTENDED);
            int n;
            if (!reti) {
              for (size_t g = 1; g < GROUPS; g++) {
                regoff_t start = match[g].rm_so;
                if (start == -1) {
                  break;
                }
                regoff_t end = match[g].rm_eo;
                char *copy = calloc(end - start + 1, sizeof(*copy));
                strncpy(copy, token3 + start, end - start);
                if (g == 1) {
                  n = atoi(copy);
                } else if (copy[0] == 'r' && n > reds) {
                  reds = n;
                } else if (copy[0] == 'g' && n > greens) {
                  greens = n;
                } else if (copy[0] == 'b' && n > blues) {
                  blues = n;
                }
                free(copy);
              }
            } else if (reti == REG_NOMATCH) {
              fprintf(stderr, "Error: no match for `%s`", token3);
              return 1;
            } else {
              fprintf(stderr, "Error: regex match failed for `%s`", token3);
              return 1;
            }
            token3 = strtok_r(nullptr, ",", &p_token3_save);
          }
          token2 = strtok_r(nullptr, ";", &p_token2_save);
        }
        int power = reds * greens * blues;
        debug("(%d, %d, %d) = %d\n", reds, greens, blues, power);
        result += power;
      }
      token = strtok_r(nullptr, ":", &p_token_save);
    }
  }
  regfree(&regex);

  return result;
}

int main(void) {
  test_case(day2, part1, sample, 8);
  test_case(day2, part1, data, 2913);
  test_case(day2, part2, sample, 2286);
  test_case(day2, part2, data, 55593);

  return 0;
}
