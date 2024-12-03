#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

constexpr size_t GROUPS = 3;

i64 part1(StringArray *lines) {
  i64 result = 0;

  regex_t regex;
  regmatch_t match[GROUPS];

  char *pattern = "mul\\(([0-9]+),([0-9]+)\\)";
  if (regcomp(&regex, pattern, REG_EXTENDED)) {
    fprintf(stderr, "Error: could not compile regex `%s`\n", pattern);
    return 1;
  }

  string_array_foreach (char *line, lines) {
    char *p = line;
    while (regexec(&regex, p, GROUPS, match, REG_EXTENDED) == 0) {
      char left[100] = {};
      char right[100] = {};
      strncpy(left, p + match[1].rm_so, match[1].rm_eo - match[1].rm_so);
      strncpy(right, p + match[2].rm_so, match[2].rm_eo - match[2].rm_so);

      i64 a = atoll(left);
      i64 b = atoll(right);

      debug("%.*s\n", match[0].rm_eo - match[0].rm_so, p + match[0].rm_so);
      debug("%s %s\n", left, right);
      debug("%lld %lld\n", a, b);

      result += a * b;

      p += match[0].rm_eo;
    }
  }

  regfree(&regex);

  return result;
}

i64 part2(StringArray *lines) {
  i64 result = 0;

  regex_t regex;
  regmatch_t match[GROUPS];

  char *pattern = "do\\(\\)|don't\\(\\)|mul\\(([0-9]+),([0-9]+)\\)";
  if (regcomp(&regex, pattern, REG_EXTENDED)) {
    fprintf(stderr, "Error: could not compile regex `%s`\n", pattern);
    return 1;
  }

  bool enabled = true;

  string_array_foreach (char *line, lines) {
    char *p = line;
    while (regexec(&regex, p, GROUPS, match, REG_EXTENDED) == 0) {
      regoff_t start = match[0].rm_so;
      regoff_t end = match[0].rm_eo;

      char instruction[100] = {};
      strncpy(instruction, p + start, end - start);
      debug("instruction: %s\n", instruction);

      if (strncmp(instruction, "do()", 4) == 0) {
        debug("enabled\n");
        enabled = true;
      } else if (strncmp(instruction, "don't()", 7) == 0) {
        debug("disable\n");
        enabled = false;
      } else if (strncmp(instruction, "mul", 3) == 0 && enabled) {
        char left[100] = {};
        char right[100] = {};
        strncpy(left, p + match[1].rm_so, match[1].rm_eo - match[1].rm_so);
        strncpy(right, p + match[2].rm_so, match[2].rm_eo - match[2].rm_so);

        i64 a = atoll(left);
        i64 b = atoll(right);

        debug("%s %s\n", left, right);
        debug("%lld %lld\n", a, b);

        result += a * b;
      }

      p += end;
    }
  }

  regfree(&regex);

  return result;
}

int main(void) {
  test_case(day3, part1, sample1, 161);
  test_case(day3, part1, data, 179834255);
  test_case(day3, part2, sample2, 48);
  test_case(day3, part2, data, 80570939);

  return 0;
}
