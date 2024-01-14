#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#include "commons.h"
#include "string.h"

StringArray *read_lines(char filename[static 1]) {
  FILE *fp = fopen(filename, "r");
  if (fp == nullptr) {
    fprintf(stderr, "Error: could not open file %s\n", filename);

    return nullptr;
  }

  StringArray *lines = string_array_new();

  char buffer[READ_LINES_BUFFER_LENGTH] = {};
  while (fgets(buffer, READ_LINES_BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    string_array_push(lines, buffer);
  }
  fclose(fp);

  return lines;
}
