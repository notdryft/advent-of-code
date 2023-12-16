#include <stdio.h>

#include "array.h"
#include "string.h"

void string_array_tests() {
  StringArray *strings = string_array_new();
  string_array_print(strings);

  string_array_push(strings, "seven");
  string_array_print(strings);
  printf("first = \"%s\", last = \"%s\"\n", string_array_first(strings), string_array_last(strings));

  string_array_push(strings, "three");
  string_array_print(strings);
  printf("first = \"%s\", last = \"%s\"\n", string_array_first(strings), string_array_last(strings));

  printf("result = [ \"%s\", \"%s\" ]\n", string_array_get(strings, 0), string_array_get(strings, 1));

  string_array_free(strings);
}

void string_split_with_one_char_delimiter_tests() {
  char *content = "one,two;three,four";

  StringArray *semi_split = string_split(content, ";");
  string_array_print(semi_split);

  for (size_t i = 0; i < semi_split->size; i++) {
    char *item = string_array_get(semi_split, i);
    StringArray *comma_split = string_split(item, ",");
    string_array_print(comma_split);

    string_array_free(comma_split);
  }

  string_array_free(semi_split);
}

void string_split_with_multiple_char_delimiter_tests() {
  char *content = "one , two ; three , four";

  StringArray *semi_split = string_split(content, " ; ");
  string_array_print(semi_split);

  for (size_t i = 0; i < semi_split->size; i++) {
    char *item = string_array_get(semi_split, i);
    StringArray *comma_split = string_split(item, " , ");
    string_array_print(comma_split);

    string_array_free(comma_split);
  }

  string_array_free(semi_split);

  content = "one: two,three,four";
  StringArray *colon_split = string_split(content, ": ");
  string_array_print(colon_split);

  StringArray *comma_split = string_split(string_array_get(colon_split, 1), ",");
  string_array_print(comma_split);

  string_array_free(comma_split);
  string_array_free(colon_split);
}

int main() {
  printf("// StringArray\n");
  string_array_tests();

  printf("\n// string_split with one char delimiter\n");
  string_split_with_one_char_delimiter_tests();

  printf("\n// string_split with multiple chars delimiter\n");
  string_split_with_multiple_char_delimiter_tests();

  return 0;
}
