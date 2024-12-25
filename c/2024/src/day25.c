#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

constexpr size_t CAPACITY = 250;
constexpr size_t KEY_HEIGHT = 5;
constexpr size_t KEY_LENGTH = 5;

i64 part1(StringArray *lines) {
  size_t locks_size = 0;
  u8 locks[CAPACITY][KEY_LENGTH] = {};

  size_t keys_size = 0;
  u8 keys[CAPACITY][KEY_LENGTH] = {};

  bool entering_new_block = true;
  bool is_lock = false;
  u8 h[KEY_LENGTH] = {};

  for (size_t j = 0; j <= lines->size; j++) {
    char *line = j == lines->size ? nullptr : lines->items[j];
    size_t line_len = j == lines->size ? 0 : strlen(line);

    if (line_len == 0) {
      u8 *d = nullptr;
      if (is_lock) d = locks[locks_size++];
      else d = keys[keys_size++];
      for (size_t i = 0; i < KEY_LENGTH; i++) {
        d[i] = h[i];
      }
#ifdef DEBUG
      if (is_lock) printf("add lock: %hu,%hu,%hu,%hu,%hu\n", d[0], d[1], d[2], d[3], d[4]);
      else printf("add key: %hu,%hu,%hu,%hu,%hu\n", d[0], d[1], d[2], d[3], d[4]);
#endif
      entering_new_block = true;
    } else {
      assert(line_len == KEY_LENGTH);
      if (entering_new_block) {
        is_lock = line[0] == '#';
        for (size_t i = 0; i < KEY_LENGTH; i++) {
          h[i] = is_lock ? 0 : KEY_HEIGHT;
        }
        entering_new_block = false;
      } else {
        for (size_t i = 0; i < KEY_LENGTH; i++) {
          if (is_lock && line[i] == '#') h[i]++;
          else if (!is_lock && line[i] == '.') h[i]--;
        }
      }
    }
  }

  i64 result = 0;
  for (size_t i = 0; i < locks_size; i++) {
    u8 *lock = locks[i];
    for (size_t j = 0; j < keys_size; j++) {
      u8 *key = keys[j];
      bool matches = true;
      for (size_t k = 0; k < KEY_LENGTH; k++) {
        matches = matches && (lock[k] + key[k] <= KEY_HEIGHT);
      }
      if (matches) {
        debug("fit: lock %hu,%hu,%hu,%hu,%hu and key %hu,%hu,%hu,%hu,%hu\n", lock[0], lock[1], lock[2], lock[3], lock[4], key[0], key[1], key[2], key[3], key[4]);
        result++;
      }
    }
  }

  return result;
}

int main(void) {
  test_case(day25, part1, sample, 3);
  test_case(day25, part1, data, 3663);

  return 0;
}
