#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"


void visit(Array *distances, i64 **graph, size_t graph_size, bool *visited, size_t current, i64 distance) {
  bool done = true;
  for (size_t next = 0; next < graph_size; next++) {
    if (graph[current][next] > 0 && !visited[next]) {
      done = false;
      visited[next] = true;
      visit(distances, graph, graph_size, visited, next, distance + graph[current][next]);
      visited[next] = false;
    }
  }
  if (done) {
    array_push_rval(distances, distance);
  }
}

i64 part1(StringArray *lines) {
  StringArray *destinations = string_array_new();

  string_array_foreach (char *line, lines) {
    char source[14], destination[14];
    if (sscanf(line, "%s to %s = %*d", source, destination) == 2) {
      if (!string_array_contains(destinations, source)) {
        string_array_push(destinations, source);
      }
      if (!string_array_contains(destinations, destination)) {
        string_array_push(destinations, destination);
      }
    }
  }
  debugf(string_array_print, destinations);

  i64 **graph = calloc(destinations->size, sizeof(*graph));
  for (size_t j = 0; j < destinations->size; j++) {
    graph[j] = calloc(destinations->size, sizeof(**graph));
  }

  string_array_foreach (char *line, lines) {
    i64 distance;
    char source[14], destination[14];
    if (sscanf(line, "%s to %s = %lld", source, destination, &distance) == 3) {
      FindResult rs = string_array_find(destinations, source);
      FindResult rd = string_array_find(destinations, destination);
      if (rs.found && rd.found) {
        graph[rs.index][rd.index] = distance;
        graph[rd.index][rs.index] = distance;
      }
    }
  }
#ifdef DEBUG
  for (size_t j = 0; j < destinations->size; j++) {
    for (size_t i = 0; i < destinations->size; i++) {
      printf("%lld\t", graph[j][i]);
    }
    printf("\n");
  }
#endif

  bool *visited = calloc(destinations->size, sizeof(*visited));
  Array *distances = array_new(i64);
  
  for (size_t i = 0; i < destinations->size; i++) {
    visited[i] = true;
    visit(distances, graph, destinations->size, visited, i, 0);
    visited[i] = false;
  }
  debugf(i64_array_print, distances);

  i64 result = INT64_MAX;
  array_foreach(i64 *distance, distances) {
    if (*distance < result) {
      result = *distance;
    }
  }

  for (size_t j = 0; j < destinations->size; j++) {
    free(graph[j]);
  }
  free(graph);
  free(visited);
  array_free(distances);
  string_array_free(destinations);

  return result;
}

i64 part2(StringArray *lines) {
  StringArray *destinations = string_array_new();

  string_array_foreach (char *line, lines) {
    char source[14], destination[14];
    if (sscanf(line, "%s to %s = %*d", source, destination) == 2) {
      if (!string_array_contains(destinations, source)) {
        string_array_push(destinations, source);
      }
      if (!string_array_contains(destinations, destination)) {
        string_array_push(destinations, destination);
      }
    }
  }
  debugf(string_array_print, destinations);

  i64 **graph = calloc(destinations->size, sizeof(*graph));
  for (size_t j = 0; j < destinations->size; j++) {
    graph[j] = calloc(destinations->size, sizeof(**graph));
  }

  string_array_foreach (char *line, lines) {
    i64 distance;
    char source[14], destination[14];
    if (sscanf(line, "%s to %s = %lld", source, destination, &distance) == 3) {
      FindResult rs = string_array_find(destinations, source);
      FindResult rd = string_array_find(destinations, destination);
      if (rs.found && rd.found) {
        graph[rs.index][rd.index] = distance;
        graph[rd.index][rs.index] = distance;
      }
    }
  }
#ifdef DEBUG
  for (size_t j = 0; j < destinations->size; j++) {
    for (size_t i = 0; i < destinations->size; i++) {
      printf("%lld\t", graph[j][i]);
    }
    printf("\n");
  }
#endif

  bool *visited = calloc(destinations->size, sizeof(*visited));
  Array *distances = array_new(i64);
  
  for (size_t i = 0; i < destinations->size; i++) {
    visited[i] = true;
    visit(distances, graph, destinations->size, visited, i, 0);
    visited[i] = false;
  }
  debugf(i64_array_print, distances);

  i64 result = -1;
  array_foreach(i64 *distance, distances) {
    if (*distance > result) {
      result = *distance;
    }
  }

  for (size_t j = 0; j < destinations->size; j++) {
    free(graph[j]);
  }
  free(graph);
  free(visited);
  array_free(distances);
  string_array_free(destinations);

  return result;
}

int main(void) {
  test_case(day9, part1, sample, 605);
  test_case(day9, part1, data, 207);
  test_case(day9, part2, sample, 982);
  test_case(day9, part2, data, 804);

  return 0;
}
