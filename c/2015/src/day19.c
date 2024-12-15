#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

typedef struct {
  char molecule[8];
  char replacement[24];
} Replacement;

i64 part1(StringArray *lines) {
  Array *replacements = array_new(Replacement);
  StringArray *results = string_array_new();

  string_array_foreach (char *line, lines) {
    size_t line_len = strlen(line);

    char molecule[8] = {};
    char replacement[24] = {};

    if (sscanf(line, "%s => %s", molecule, replacement) == 2) {
      Replacement r = {};
      strncpy(r.molecule, molecule, 8);
      strncpy(r.replacement, replacement, 24);
      array_push(replacements, &r);

      debug("|%s| => |%s|\n", r.molecule, r.replacement);
    } else if (line_len > 0) {
      debug("line: %s\n", line);
      for (size_t i = 0; i < line_len; i++) {
        trace("i = %zu\n", i);
        array_foreach (Replacement *r, replacements) {
          size_t molecule_len = strlen(r->molecule);
          size_t replacement_len = strlen(r->replacement);
          trace("%s (%zu) => %s (%zu)\n", r->molecule, molecule_len, r->replacement, replacement_len);
          if (strncmp(line + i, r->molecule, molecule_len) == 0) {
            debug("matched %.*s at %zu with `%s => %s`\n", (int) molecule_len, line + i, i, r->molecule, r->replacement);

            char result[1000] = {};
            strncpy(result, line, i);
            strncpy(result + i, r->replacement, replacement_len);
            strncpy(result + i + replacement_len, line + i + molecule_len, line_len - i - molecule_len);
            debug("resulting in `%s`\n", result);

            if (!string_array_contains(results, result)) {
              string_array_push(results, result);
            }
          }
        }
      }
    }
  }

  array_free(replacements);
  string_array_free(results);

  return results->size;
}

bool find_longest_replacement(char *line, size_t line_len, Array *replacements, size_t *i, size_t *j) {
  ssize_t besti = -1, bestlen = -1, bestj = -1;
  for (size_t ii = 0; ii < line_len; ii++) {
    array_enumerate (Replacement *r, jj, replacements) {
      ssize_t replacement_len = strlen(r->replacement);
      if (strncmp(line + ii, r->replacement, replacement_len) == 0) {
        if (replacement_len > bestlen) {
          besti = ii;
          bestlen = replacement_len;
          bestj = jj;
        }
      }
    }
  }
  if (besti != -1 && bestlen != -1 && bestj != -1) {
    *i = besti;
    *j = bestj;
    return true;
  }
  return false;
}

i64 part2(StringArray *lines) {
  i64 result = 0;

  Array *replacements = array_new(Replacement);
  string_array_foreach (char *line, lines) {
    size_t line_len = strlen(line);

    char molecule[3] = {};
    char replacement[20] = {};

    if (sscanf(line, "%s => %s", molecule, replacement) == 2) {
      Replacement r = {};
      strncpy(r.molecule, molecule, 7);
      strncpy(r.replacement, replacement, 23);
      array_push(replacements, &r);

      debug("|%s| => |%s|\n", r.molecule, r.replacement);
    } else if (line_len > 0) {
      debug("%s\n", line);

      char *chain = strdup(line);
      size_t chain_len = strlen(chain);

      while (chain_len > 1 || chain[0] != 'e') {
        size_t i, j;
        if (find_longest_replacement(chain, chain_len, replacements, &i, &j)) {
          result++;

          Replacement *r = array_get(replacements, j);
          size_t molecule_len = strlen(r->molecule);
          size_t replacement_len = strlen(r->replacement);
          debug("trying %s => %s from %zu\n", r->replacement, r->molecule, i);

          char *tmp = calloc(1000, sizeof(char));
          strncpy(tmp, chain, i);
          #pragma GCC diagnostic ignored "-Wstringop-truncation"
          strncpy(tmp + i, r->molecule, molecule_len);
          #pragma GCC diagnostic ignored "-Wstringop-truncation"
          strncpy(tmp + i + molecule_len, chain + i + replacement_len, chain_len - i - replacement_len);
          debug("%s\n", tmp);

          size_t tmp_len = strlen(tmp);

          free(chain);
          chain = tmp;
          chain_len = tmp_len;
        } else {
          break;
        }
      }
      free(chain);
    }
  }

  array_free(replacements);

  return result;
}

int main(void) {
  test_case(day19, part1, sample1, 4);
  test_case(day19, part1, data, 535);
  test_case(day19, part2, sample2, 6);
  test_case(day19, part2, data, 212);

  return 0;
}
