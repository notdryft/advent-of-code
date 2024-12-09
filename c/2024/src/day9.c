#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

i64 part1(StringArray *lines) {
  i64 result = 0;

  string_array_foreach (char *line, lines) {
    size_t line_len = strlen(line);
    debug("%s: %zu\n", line, line_len);

    size_t disk_len = 0;
    for (size_t i = 0; i < line_len; i++) {
      char block_len = line[i] - '0';
      disk_len += block_len;
    }
    debug("disk_len: %zu\n", disk_len);

    Array *disk = array_new(i64);

    i64 block_id = 0;
    i64 empty_block = -1;

    bool is_file = true;
    for (size_t i = 0; i < line_len; i++) {
      size_t block_len = line[i] - '0';
      for (size_t j = 0; j < block_len; j++) {
        if (is_file) {
          array_push_rval(disk, block_id);
        } else {
          array_push_rval(disk, empty_block);
        }
      }
      if (is_file) block_id++;
      is_file = !is_file;
    }
    debugf(i64_array_print, disk);

    for (size_t i = 0; i < disk->size; i++) {
      i64 block = i64_array_get(disk, i);
      if (block == empty_block) {
        i64 id = i64_array_last(disk);
        array_remove_last(disk);
        while (id == empty_block) {
          id = i64_array_last(disk);
          array_remove_last(disk);
        }
        array_set_rval(disk, i, id);
      }
    }
    debugf(i64_array_print, disk);

    for (size_t i = 0; i < disk->size; i++) {
      i64 id = i64_array_get(disk, i);
      result += i * id;
    }

    array_free(disk);
  }

  return result;
}

typedef struct {
  i64 id;
  i64 position;
  size_t len;
} Block;

Block find_block(Array *array, i64 id, size_t start) {
  for (size_t i = start; i < array->size; i++) {
    i64 current = i64_array_get(array, i);
    if (current == id) {
      size_t j = i;
      while (current == id) {
        current = i64_array_get(array, j++);
      }
      return (Block) { .id = id, .position = i, .len = j - i - 1 };
    }
  }

  return (Block) { .position = -1 };
}

Block find_first_available_empty_space(Array *array, i64 empty_block_id, size_t len) {
  for (size_t i = 0; i < array->size; i++) {
    i64 current = i64_array_get(array, i);
    if (current == empty_block_id) {
      size_t j = i;
      while (current == empty_block_id) {
        current = i64_array_get(array, j++);
      }
      size_t current_len = j - i - 1;
      if (len <= current_len) {
        return (Block) { .id = empty_block_id, .position = i, .len = j - i - 1 };
      } else {
        i += current_len;
      }
    }
  }
  
  return (Block) { .position = -1 };
}

i64 part2(StringArray *lines) {
  i64 result = 0;

  string_array_foreach (char *line, lines) {
    size_t line_len = strlen(line);
    debug("%s: %zu\n", line, line_len);

    size_t disk_len = 0;
    for (size_t i = 0; i < line_len; i++) {
      char block_len = line[i] - '0';
      disk_len += block_len;
    }
    debug("disk_len: %zu\n", disk_len);

    Array *disk = array_new(i64);

    i64 block_id = 0;
    i64 empty_block_id = -1;

    bool is_file = true;
    for (size_t i = 0; i < line_len; i++) {
      size_t block_len = line[i] - '0';
      for (size_t j = 0; j < block_len; j++) {
        if (is_file) {
          array_push_rval(disk, block_id);
        } else {
          array_push_rval(disk, empty_block_id);
        }
      }
      if (is_file) block_id++;
      is_file = !is_file;
    }
    debugf(i64_array_print, disk);

    block_id--;
    while (block_id > 0) {
      debug("block id: %lld\n", block_id);
      Block block = find_block(disk, block_id, 0);
      debug("block: { %lld %lld %zu }\n", block.id, block.position, block.len);
      Block empty_block = find_first_available_empty_space(disk, empty_block_id, block.len);
      debug("empty_block: { %lld %lld %zu }\n", empty_block.id, empty_block.position, empty_block.len);
      if (block.position < empty_block.position) {
        block_id--;
        continue;
      }
      if (block.len <= empty_block.len) {
        size_t len = block.len;
        for (size_t i = 0; i < len; i++) {
          array_set_rval(disk, block.position + i, empty_block_id);
          array_set_rval(disk, empty_block.position + i, block.id);
        }
        i64 last = i64_array_last(disk);
        while (last < 0) {
          array_remove_last(disk);
          last = i64_array_last(disk);
        }
      }
      block_id--;
      debugf(i64_array_print, disk);
    }

    for (size_t i = 0; i < disk->size; i++) {
      i64 id = i64_array_get(disk, i);
      if (id > 0) {
        result += i * id;
      }
    }

    array_free(disk);
  }

  return result;
}

int main(void) {
  test_case(day9, part1, sample, 1928);
  test_case(day9, part1, data, 6258319840548);
  test_case(day9, part2, sample, 2858);
  test_case(day9, part2, data, 6286182965311);

  return 0;
}
