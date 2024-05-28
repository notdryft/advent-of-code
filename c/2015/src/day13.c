#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

void visit(Array *values, i64 **graph, size_t graph_size, bool *visited, Array *paths) {
  bool done = true;
  for (size_t current = 0; current < graph_size; current++) {
    if (!visited[current]) {
      done = false;
      visited[current] = true;
      array_push_rval(paths, current);
      visit(values, graph, graph_size, visited, paths);
      array_remove_last(paths);
      visited[current] = false;
    }
  }
  if (done) {
    debugf(i64_array_print, paths);

    i64 value = 0;
    for (size_t i = 0; i < paths->size; i++) {
      i64 prev = i == 0 ? i64_array_last(paths) : i64_array_get(paths, i - 1);
      i64 current = i64_array_get(paths, i);
      i64 next = i == paths->size - 1 ? i64_array_first(paths) : i64_array_get(paths, i + 1);
      debug("%lld: %lld %lld\n", current, graph[current][prev], graph[current][next]);
      value += graph[current][prev] + graph[current][next];
    }
    debug("value = %lld\n", value);

    array_push_rval(values, value);
  }
}

i64 compute(StringArray *lines, bool include_self) {
  StringArray *guests = string_array_new();
  if (include_self) {
    string_array_push(guests, "Me");
  }
  string_array_foreach(char *line, lines) {
    StringArray *split = string_split(line, " ");
    char *name = string_array_get(split, 0);
    if (!string_array_contains(guests, name)) {
      string_array_push(guests, name);
    }
    string_array_free(split);
  }

  i64 **graph = calloc(guests->size, sizeof(*graph));
  for (size_t j = 0; j < guests->size; j++) {
    graph[j] = calloc(guests->size, sizeof(**graph));
  }

  string_array_foreach (char *line, lines) {
    i64 amount;
    char name1[10], name2[10], verb[5];

    if (sscanf(line, "%s would %s %lld happiness units by sitting next to %[a-zA-Z]s.", name1, verb, &amount, name2) == 4) {
      if (strncmp(verb, "lose", 4) == 0) {
        amount = -amount;
      }

      FindResult r1 = string_array_find(guests, name1);
      FindResult r2 = string_array_find(guests, name2);
      if (r1.found && r2.found) {
        graph[r1.index][r2.index] = amount;
      }
    }
  }
#ifdef DEBUG
  for (size_t j = 0; j < guests->size; j++) {
    for (size_t i = 0; i < guests->size; i++) {
      printf("%4lld\t", graph[j][i]);
    }
    printf("\n");
  }
#endif

  bool *visited = calloc(guests->size, sizeof(*visited));
  Array *paths = array_new(i64);
  Array *values = array_new(i64);

  for (size_t current = 0; current < guests->size; current++) {
    //size_t current = 0;
    visited[current] = true;
    array_push_rval(paths, current);
    visit(values, graph, guests->size, visited, paths);
    array_remove_last(paths);
    visited[current] = false;
  }
  debugf(i64_array_print, values);

  i64 result = -1;
  array_foreach(i64 *happiness, values) {
    if (*happiness > result) {
      result = *happiness;
    }
  }

  array_free(paths);
  array_free(values);
  for (size_t j = 0; j < guests->size; j++) {
    free(graph[j]);
  }
  free(graph);
  free(visited);
  string_array_free(guests);

  return result;
}

i64 part1(StringArray *lines) {
  return compute(lines, false);
}

i64 part2(StringArray *lines) {
  return compute(lines, true);
}

int main(void) {
  test_case(day13, part1, sample, 330);
  test_case(day13, part1, data, 664);
  test_case(day13, part2, data, 640);

  return 0;
}
