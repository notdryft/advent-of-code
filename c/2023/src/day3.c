#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commons.h"
#include "string.h"

bool is_digit(char c) {
  return '0' <= c && c <= '9';
}

bool is_empty(char c) {
  return c == '.' || c == 'D' || c == 'N' || c == 'P' || c == '\0';
}

bool is_symbol(char c) {
  return c == '-' || c == '#' || c == '=' || c == '*' || c == '+' ||
         c == '@' || c == '$' || c == '&' || c == '/' || c == '%';
}

bool is_gear_symbol(char c) {
  return c == '*';
}

int part1(StringArray *lines) {
  int result = 0;

  string_array_enumerate (char *line, j, lines) {
    size_t line_len = strlen(line);
    debug("%s: %zu\n", line, line_len);

    size_t digit_so = SIZE_MAX;
    for (size_t i = 0; i <= line_len; i++) {
      if (is_digit(line[i]) && digit_so == SIZE_MAX) {
        digit_so = i;
      } else if (!is_digit(line[i]) && digit_so != SIZE_MAX) {
        size_t digit_eo = i;
        size_t digit_len = digit_eo - digit_so;
        char *digit_str = calloc(digit_len + 1, sizeof(*digit_str));
        strncpy(digit_str, line + digit_so, digit_len);

        size_t sy = ((long) j - 1 < 0) ? 0 : j - 1;
        size_t ey = (j + 1 >= lines->size) ? j : j + 1;
        size_t sx = ((long) digit_so - 1 < 0) ? 0 : digit_so - 1;
        size_t ex = (digit_eo + 1 > line_len) ? digit_eo : digit_eo + 1;

        bool is_near_symbol = false;
        for (size_t y = sy; y <= ey; y++) {
          char *sline = string_array_get(lines, y);
          for (size_t x = sx; x < ex; x++) {
            if (is_symbol(sline[x])) {
              is_near_symbol = true;
            }
          }
        }

        if (is_near_symbol) {
          result += atoi(digit_str);
          debug("digit: %s (%zu, %zu) is next to a symbol\n", digit_str, digit_so, digit_eo);
        }

        digit_so = SIZE_MAX;

        free(digit_str);
      }
    }
  }

  return result;
}

int part2(StringArray *lines) {
  int result = 0;

  size_t lol_x = SIZE_MAX;
  size_t lol_y = SIZE_MAX;

  string_array_enumerate (char *line, j, lines) {
    size_t line_len = strlen(line);
    debug("%s: %zu\n", line, line_len);

    size_t digit_so = SIZE_MAX;
    for (size_t i = 0; i <= line_len; i++) {
      if (is_digit(line[i]) && digit_so == SIZE_MAX) {
        digit_so = i;
      } else if (!is_digit(line[i]) && digit_so != SIZE_MAX) {
        size_t digit_eo = i;
        size_t digit_len = digit_eo - digit_so;
        char *digit_str = calloc(digit_len + 1, sizeof(*digit_str));
        strncpy(digit_str, line + digit_so, digit_len);

        for (size_t id = digit_so; id < digit_eo; id++) {
          line[id] = 'N';
        }

        size_t sy = ((long) j - 1 < 0) ? 0 : j - 1;
        size_t ey = (j + 1 >= lines->size) ? j : j + 1;
        size_t sx = ((long) digit_so - 1 < 0) ? 0 : digit_so - 1;
        size_t ex = (digit_eo + 1 > line_len) ? digit_eo : digit_eo + 1;

        size_t gear_x;
        size_t gear_y;
        bool is_near_gear_symbol = false;
        for (size_t y = sy; y <= ey; y++) {
          char *sline = string_array_get(lines, y);
          for (size_t x = sx; x < ex; x++) {
            if (is_gear_symbol(sline[x])) {
              is_near_gear_symbol = true;
              gear_x = x;
              gear_y = y;
            }
          }
        }

        if (is_near_gear_symbol) {
          if (lol_x == gear_x && lol_y == gear_y) {
            debug("(%zu, %zu)", lol_x, lol_y);
            exit(42);
          }

          size_t nsy = ((long) gear_y - 1 < 0) ? 0 : gear_y - 1;
          size_t ney = (gear_y + 1 >= lines->size) ? gear_y : gear_y + 1;
          size_t nsx = ((long) gear_x - 1 < 0) ? 0 : gear_x - 1;
          size_t nex = (gear_x + 1 >= line_len) ? gear_x : gear_x + 1;

          debug("scanning from (%zu, %zu) to (%zu, %zu)\n", nsx, nsy, nex, ney);

          size_t digit_x;
          size_t digit_y;
          bool is_gear_near_another_digit = false;
          debug("nline: ");
          for (size_t y = nsy; y <= ney; y++) {
            char *nline = string_array_get(lines, y);
            for (size_t x = nsx; x <= nex; x++) {
              debug("%c", nline[x]);
              if (is_digit(nline[x])) {
                is_gear_near_another_digit = true;
                digit_x = x;
                digit_y = y;
              }
            }
          }
          debug("\n");

          debug("digit: %s (%zu, %zu) is next to a symbol (%zu, %zu)\n", digit_str, digit_so, digit_eo, gear_x, gear_y);

          if (is_gear_near_another_digit) {
            char *pline = string_array_get(lines, digit_y);
            size_t pair_so = digit_x;
            size_t pair_eo = digit_x;
            while (is_digit(pline[pair_so])) {
              pair_so--;
            }
            pair_so++;
            while (is_digit(pline[pair_eo])) {
              pair_eo++;
            }

            size_t pair_len = pair_eo - pair_so;
            char *pair_str = calloc(pair_len + 1, sizeof(*pair_str));
            strncpy(pair_str, pline + pair_so, pair_len);

            for (size_t id = digit_so; id < digit_eo; id++) {
              line[id] = 'D';
            }
            for (size_t ip = pair_so; ip < pair_eo; ip++) {
              pline[ip] = 'P';
            }

            debug("paired digit: %s (%zu, %zu) from symbol (%zu, %zu)\n", pair_str, digit_x, digit_y, gear_x, gear_y);
            result += atoi(digit_str) * atoi(pair_str);
            free(pair_str);

            lol_x = gear_x;
            lol_y = gear_y;
          }
        }

        digit_so = SIZE_MAX;

        free(digit_str);
      }
    }
  }

  debug("\n");
  for (size_t i = 0; i < lines->size; i++) {
    debug("%s\n", string_array_get(lines, i));
  }

  return result;
}

int main(void) {
  test_case(day3, part1, sample, 4361);
  test_case(day3, part1, data, 556367);
  test_case(day3, part2, sample, 467835);
  test_case(day3, part2, data, 89471771);

  return 0;
}
