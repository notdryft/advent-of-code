#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

bool is_vowel(char c) {
  return c == 'a' || c == 'e' || c == 'u' || c == 'i' || c == 'o';
}

bool is_banned_chain(char *str, size_t index) {
  return (str[index] == 'a' && str[index + 1] == 'b') ||
    (str[index] == 'c' && str[index + 1] == 'd') ||
    (str[index] == 'p' && str[index + 1] == 'q') ||
    (str[index] == 'x' && str[index + 1] == 'y');
}

i64 part1(StringArray *lines) {
  i64 result = 0;

  string_array_foreach (char *line, lines) {
    size_t line_len = strlen(line);
    debug("%s: %zu\n", line, line_len);

    i32 vowels = 0;
    bool has_double_letters = false;
    bool has_banned_chains = false;

    for (size_t i = 0; i < line_len; i++) {
      if (is_vowel(line[i])) {
        vowels++;
      }
      if (i < line_len - 1) {
        if (line[i] == line[i + 1]) {
          has_double_letters = true;
        }
        if (is_banned_chain(line, i)) {
          has_banned_chains = true;
        }
      }
    }
    debug("vowels: %d\n", vowels);
    debug("has_double_letters: %s\n", has_double_letters ? "true" : "false");
    debug("has_banned_chains: %s\n", has_banned_chains ? "true" : "false");

    if (!has_banned_chains && has_double_letters && vowels >= 3) {
      result += 1;
    }
  }

  return result;
}

i64 part2(StringArray *lines) {
  i64 result = 0;

  string_array_foreach (char *line, lines) {
    size_t line_len = strlen(line);
    debug("%s: %zu\n", line, line_len);

    bool has_repeat = false;
    bool has_twice = false;

    for (size_t i = 0; i < line_len - 1; i++) {
      for (size_t j = i + 2; j < line_len - 1; j++) {
        if (line[i] == line[j] && line[i + 1] == line[j + 1]) {
          has_twice = true;
          break;
        }
      }
      if (i < line_len - 2) {
        if (line[i] == line[i + 2]) {
          has_repeat = true;
        }
      }
    }
    debug("has_twice: %s\n", has_twice ? "true" : "false");
    debug("has_repeat: %s\n", has_repeat ? "true" : "false");

    if (has_repeat && has_twice) {
      result += 1;
    }
  }

  return result;
}

int main(void) {
  test_case(day5, part1, sample1, 2);
  test_case(day5, part1, data, 238);
  test_case(day5, part2, sample2, 2);
  test_case(day5, part2, data, 69);

  return 0;
}
