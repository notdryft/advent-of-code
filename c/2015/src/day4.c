#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include <openssl/md5.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

void md5_hash_from_string(char *str, size_t str_len, char *hash) {
  unsigned char md5[MD5_DIGEST_LENGTH] = { 0 };

  MD5((const unsigned char *) str, str_len, md5);

  for (size_t i = 0; i < MD5_DIGEST_LENGTH; i++) {
    sprintf(hash + 2 * i, "%02x", md5[i]);
  }
}

i64 part1(StringArray *lines) {
  i64 result = 0;

  char md5_hash[2 * MD5_DIGEST_LENGTH + 1] = {};

  string_array_foreach (char *line, lines) {
    size_t line_len = strlen(line);

    char str[100] = {};
    strncpy(str, line, line_len);
    debug("str: %s\n", str);

    for (i32 i = 1; i < INT32_MAX; i++) {
      sprintf(str + line_len, "%d", i);
      md5_hash_from_string(str, strlen(str), md5_hash);
      debug("%s: %s\n", str, md5_hash);

      if (
        md5_hash[0] == '0' &&
        md5_hash[1] == '0' &&
        md5_hash[2] == '0' &&
        md5_hash[3] == '0' &&
        md5_hash[4] == '0'
      ) {
        result = i;
        break;
      }
    }
  }

  return result;
}

i64 part2(StringArray *lines) {
  i64 result = 0;

  char md5_hash[2 * MD5_DIGEST_LENGTH + 1] = {};

  string_array_foreach (char *line, lines) {
    size_t line_len = strlen(line);

    char str[100] = {};
    strncpy(str, line, line_len);
    debug("str: %s\n", str);

    for (i32 i = 1; i < INT32_MAX; i++) {
      sprintf(str + line_len, "%d", i);
      md5_hash_from_string(str, strlen(str), md5_hash);
      debug("%s: %s\n", str, md5_hash);

      if (
        md5_hash[0] == '0' &&
        md5_hash[1] == '0' &&
        md5_hash[2] == '0' &&
        md5_hash[3] == '0' &&
        md5_hash[4] == '0' &&
        md5_hash[5] == '0'
      ) {
        result = i;
        break;
      }
    }
  }

  return result;
}

int main(void) {
  test_case(day4, part1, sample1, 609043);
  test_case(day4, part1, sample2, 1048970);
  test_case(day4, part1, data, 117946);
  test_case(day4, part2, data, 3938038);

  return 0;
}
