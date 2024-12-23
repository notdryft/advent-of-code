#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

constexpr size_t GRAPH_SIZE = 26 * 26;

#define hash(key) (key[0]-'a'+(key[1]-'a')*26)

i64 part1(StringArray *lines) {
  i64 result = 0;

  bool **graph = malloc(sizeof(*graph) * GRAPH_SIZE);
  for (size_t j = 0; j < GRAPH_SIZE; j++) {
    graph[j] = calloc(GRAPH_SIZE, sizeof(**graph));
  }
  StringArray *nodes = string_array_new();

  string_array_foreach (char *line, lines) {
    char left[3] = {};
    char right[3] = {};
    
    if (sscanf(line, "%[^-]-%s", left, right) == 2) {
      debug("%s -> %s\n", left, right);

      size_t lh = hash(left);
      size_t rh = hash(right);
      graph[lh][rh] = true;
      graph[rh][lh] = true;

      if (!string_array_contains(nodes, left)) {
        string_array_push(nodes, left);
      }
      if (!string_array_contains(nodes, right)) {
        string_array_push(nodes, right);
      }
    }
  }
  debugf(string_array_print, nodes);

  for (size_t i = 0; i < nodes->size; i++) {
    char *middle = nodes->items[i];
    size_t mh = hash(middle);
    for (size_t j = i + 1; j < nodes->size; j++) {
      char *left = nodes->items[j];
      size_t lh = hash(left);
      for (size_t k = j + 1; k < nodes->size; k++) {
        char *right = nodes->items[k];
        size_t rh = hash(right);
        if (graph[mh][lh] && graph[mh][rh] && graph[lh][rh] && (middle[0] == 't' || left[0] == 't' || right[0] == 't')) {
          debug("%s,%s,%s\n", left, middle, right);
          result++;
        }
      }
    }
  }

  for (size_t j = 0; j < GRAPH_SIZE; j++) {
    free(graph[j]);
  }
  free(graph);
  string_array_free(nodes);

   return result;
}

void interconnected_rec(bool **graph, StringArray *nodes, Array *connections, StringArray *acc, size_t k, size_t h, size_t d, size_t depth) {
  if (d == depth) {
    bool connected = true;
    for (size_t i = 0; i < acc->size; i++) {
      char *left = acc->items[i];
      size_t lh = hash(left);
      for (size_t j = i + 1; j < acc->size; j++) {
        char *right = acc->items[j];
        size_t rh = hash(right);
        connected = connected && graph[lh][rh];
      }
    }
    if (connected) {
      StringArray *tmp = string_array_dup(acc);
      array_push_pointer(connections, tmp);
    }
  } else {
    for (size_t i = k + 1; i < nodes->size; i++) {
      char *new_node = nodes->items[i];
      size_t nh = hash(new_node);
      if (graph[h][nh]) {
        string_array_push(acc, new_node);
        interconnected_rec(graph, nodes, connections, acc, i, h, d + 1, depth);
        string_array_remove_last(acc);
      }
    }
  }
}

Array *interconnected(bool **graph, StringArray *nodes, size_t depth) {
  Array *connections = array_new(StringArray *);
  for (size_t i = 0; i < nodes->size; i++) {
    char *node = nodes->items[i];
    size_t h = hash(node);
    StringArray *acc = string_array_new();
    string_array_push(acc, node);
    interconnected_rec(graph, nodes, connections, acc, i , h, 1, depth);
    string_array_free(acc);
  }
  return connections;
}

int node_cmp(const void *a, const void *b) {
  const char *a_str = *(const char **) a;
  const char *b_str = *(const char **) b;
  return strcmp(a_str, b_str);
}

i64 part2(StringArray *lines, char *expected) {
  bool **graph = malloc(sizeof(*graph) * GRAPH_SIZE);
  for (size_t j = 0; j < GRAPH_SIZE; j++) {
    graph[j] = calloc(GRAPH_SIZE, sizeof(**graph));
  }
  StringArray *nodes = string_array_new();

  string_array_foreach (char *line, lines) {
    char left[3] = {};
    char right[3] = {};
    
    if (sscanf(line, "%[^-]-%s", left, right) == 2) {
      debug("%s -> %s\n", left, right);

      size_t lh = hash(left);
      size_t rh = hash(right);
      graph[lh][rh] = true;
      graph[rh][lh] = true;

      if (!string_array_contains(nodes, left)) {
        string_array_push(nodes, left);
      }
      if (!string_array_contains(nodes, right)) {
        string_array_push(nodes, right);
      }
    }
  }
  debugf(string_array_print, nodes);

  size_t i = 4;
  Array *connections = interconnected(graph, nodes, i);
  while (connections->size != 1) {
    for (size_t j = 0; j < connections->size; j++) {
      StringArray *array = array_get_pointer(connections, j);
      string_array_free(array);
    }
    array_free(connections);
    connections = interconnected(graph, nodes, ++i);
  }

  i64 result = -1;
  char result_str[100] = {};

  if (connections->size == 1) {
    StringArray *array = array_get_pointer(connections, 0);
    result = array->size;

    qsort(array->items, array->size, sizeof(char *), node_cmp);
    for (size_t j = 0; j < array->size; j++) {
      if (j > 0) {
        size_t k = (j - 1) * 3 + 2;
        result_str[k] = ',';
        strncpy(result_str + k + 1, array->items[j], 2);
      } else {
        strncpy(result_str, array->items[j], 2);
      }
    }
  }
 
  assert(result_str != nullptr);
  assert(strncmp(result_str, expected, strlen(expected)) == 0);
  assert(strlen(result_str) == strlen(expected));

  for (size_t i = 0; i < connections->size; i++) {
    StringArray *array = array_get_pointer(connections, i);
    string_array_free(array);
  }
  array_free(connections);
  for (size_t j = 0; j < GRAPH_SIZE; j++) {
    free(graph[j]);
  }
  free(graph);
  string_array_free(nodes);

  return result;
}

int main(void) {
  test_case(day23, part1, sample, 7);
  test_case(day23, part1, data, 1064);
  test_case(day23, part2, sample, 4, "co,de,ka,ta");
  test_case(day23, part2, data, 13, "aq,cc,ea,gc,jo,od,pa,rg,rv,ub,ul,vr,yy");

  return 0;
}
