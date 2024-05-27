#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

bool is_part_of_digit(char c) {
  return c == '-' || (c >= '0' && c <= '9');
}

i64 part1(StringArray *lines) {
  i64 result = 0;

  string_array_foreach (char *line, lines) {
    size_t line_len = strlen(line);

    char *p = line;
    for (size_t i = 0; i < line_len; i++) {
      if (is_part_of_digit(line[i])) {
        p = &line[i];
        size_t len = i;
        while (is_part_of_digit(line[i + 1])) {
          i++;
        }
        len = i - len + 1;
        char *str = calloc(len, sizeof(char));
        strncpy(str, p, len);
        i64 number = atoll(str);
        free(str);
        debug("found %lld\n", number);
        result += number;
      }
    }
  }

  return result;
}

typedef struct {
  Array *children;
  i64 number;
  bool red;
} Json;

void json_free(Array *array) {
  for (size_t i = 0; i < array->size; i++) {
    Json *json = array_get(array, i);
    if (json->children != nullptr) {
      json_free(json->children);
    }
  }
  array_free(array);
}

void indent(size_t depth) {
  for (size_t i = 0; i < depth; i++) {
    printf("  ");
  }
}

void print_json_with_depth(Array *array, size_t depth) {
  printf("Json{\n");
  for (size_t i = 0; i < array->size; i++) {
    Json *json = array_get(array, i);
    indent(depth); printf("  %zu -> ", i);
    if (json->children == nullptr) {
      if (json->red) {
        printf("RED\n");
      } else {
        printf("%lld\n", json->number);
      }
    } else {
      print_json_with_depth(json->children, depth + 1);
    }
  }
  indent(depth); printf("} size = %zu\n", array->size);
}

void print_json(Json root) {
  print_json_with_depth(root.children, 0);
}

void print_stack(Array **stack, size_t stack_size) {
  printf("Stack{\n");
  for (size_t i = 0; i < stack_size; i++) {
    Array *array = stack[i];
    printf("  %zu -> ", i);
    print_json_with_depth(array, 1);
  }
  printf("}\n");
}

bool json_has_red(Array *array) {
  for (size_t i = 0; i < array->size; i++) {
    Json *json = array_get(array, i);
    if (json->red) {
      return true;
    }
  }
  return false;
}

i64 json_sum(Array *array) {
  if (json_has_red(array)) {
    return 0;
  }

  i64 sum = 0;
  for (size_t i = 0; i < array->size; i++) {
    Json *json = array_get(array, i);
    if (json->children == nullptr) {
      sum += json->number;
    } else {
      sum += json_sum(json->children);
    }
  }

  return sum;
}

i64 part2(StringArray *lines) {
  i64 result = 0;

  string_array_foreach (char *line, lines) {
    size_t line_len = strlen(line);
    debug("%s: %zu\n", line, line_len);

    Json root = { .children = array_new(Json) };
    tracef(print_json, root);

    size_t stack_capacity = 1000;
    size_t stack_size = 0;
    Array **stack = calloc(stack_capacity, sizeof(Array **));
    stack[stack_size++] = root.children;

    for (size_t i = 0; i < line_len; i++) {
      trace("reading: %c\n", line[i]);

      Array *current = stack[stack_size - 1];
      if (line[i] == ':' && line[i + 1] == '"' && line[i + 2] == 'r' && line[i + 3] == 'e' && line[i + 4] == 'd') {
        i += 4;

        array_push(current, &(Json) { .red = true });
      } else if (line[i] == '{') {
        Json node = { .children = array_new(Json) };
        array_push_rval(current, node);

        stack[stack_size++] = node.children;
      } else if (line[i] == '}') {
        stack_size--;
      } else if (is_part_of_digit(line[i])) {
        char *p = &line[i];
        size_t len = i;
        while (is_part_of_digit(line[i + 1])) {
          i++;
        }
        len = i - len + 1;
        char *str = calloc(len, sizeof(char));
        strncpy(str, p, len);
        i64 number = atoll(str);
        free(str);

        array_push(current, &(Json) { .number = number });
      }
      tracef(print_json, root);
    }

    result += json_sum(root.children);

    json_free(root.children);
    free(stack);
  }

  return result;
}

int main(void) {
  test_case(day12, part1, data, 191164);
  test_case(day12, part2, sample, 31);
  test_case(day12, part2, data, 87842);

  return 0;
}
