#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "string.h"

#define HASHES 26*26*26
#define hash(key) (key[0]-'a'+(key[1]-'a')*26+(key[2]-'a')*26*26)
#define reverse_hash(hash) {hash%26+'a',(hash/26)%26+'a',(hash/(26*26))%26+'a','\0'}

int contract(int **graph, size_t graph_size, bool *merged, bool *visited, int *weights, size_t *s, size_t *t) {
  int mincut = 0;
  int k, maxc;
  for (size_t i = 0; i < graph_size; i++) {
    weights[i] = visited[i] = 0;
  }
  for (size_t i = 0; i < graph_size; i++) {
    k = -1;
    maxc = -1;
    for (size_t j = 0; j < graph_size; j++) {
      if (!merged[j] && !visited[j] && weights[j] > maxc) {
        k = j;
        maxc = weights[j];
      }
    }
    if (k == -1) {
      return mincut;
    }
    *s = *t;
    *t = k;
    mincut = maxc;
    visited[k] = true;
    for (size_t j = 0; j < graph_size; j++) {
      if (!merged[j] && !visited[j]) {
        weights[j] += graph[k][j];
      }
    }
  }
  return mincut;
}

// https://en.wikipedia.org/wiki/Stoer%E2%80%93Wagner_algorithm
int stoer_wagner(int **graph, size_t graph_size) {
  int *contractions = calloc(graph_size, sizeof(*contractions));
  bool *merged = calloc(graph_size, sizeof(*merged));
  bool *visited = calloc(graph_size, sizeof(*visited));
  int *weights = calloc(graph_size, sizeof(*weights));

  for (size_t i = 0; i < graph_size - 1; i++) {
    contractions[i] = 1;
  }

  size_t s = 0, t = 0;
  int result = -1;
  for (size_t i = 0; i < graph_size - 1; i++) {
    int contracted = contract(graph, graph_size, merged, visited, weights, &s, &t);
    merged[t] = true;

    if (contracted == 3) {
      result = contractions[s] + contractions[t];
      result = result * (graph_size - result);
      goto defer;
    }
    contractions[s] += contractions[t];

    for (size_t j = 0; j < graph_size; j++) {
      if (!merged[j]) {
        graph[j][s] += graph[j][t];
        graph[s][j] = graph[j][s];
      }
    }
  }

  defer:
  free(contractions);
  free(merged);
  free(visited);
  free(weights);

  return result;
}

void set_add(StringArray *array, char *name) {
  if (!string_array_contains(array, name)) {
    string_array_push(array, name);
  }
}

int part1(StringArray *lines) {
  size_t hashes_size = HASHES;
  int **hashes = malloc(sizeof(*hashes) * hashes_size);
  for (size_t j = 0; j < hashes_size; j++) {
    hashes[j] = calloc(hashes_size, sizeof(**hashes));
  }
  StringArray *nodes = string_array_new();

  string_array_foreach (char *line, lines) {
    StringArray *split = string_split(line, ": ");
    StringArray *ssplit = string_split(split->items[1], " ");

    char *u_str = split->items[0];
    size_t u = hash(u_str);
    set_add(nodes, u_str);
    for (size_t i = 0; i < ssplit->size; i++) {
      char *v_str = ssplit->items[i];
      size_t v = hash(v_str);
      hashes[u][v] = 1;
      hashes[v][u] = 1;
      set_add(nodes, v_str);
    }

    string_array_free(ssplit);
    string_array_free(split);
  }

  int *permutations = calloc(hashes_size, sizeof(*permutations));
  // Algorithm seems to be sensible to nodes ordering?
  for (size_t i = 0; i < nodes->size; i++) {
    int h = hash(nodes->items[i]);
    permutations[h] = i;
  }

  size_t graph_size = nodes->size;
  int **graph = malloc(sizeof(*graph) * graph_size);
  for (size_t j = 0; j < graph_size; j++) {
    graph[j] = calloc(graph_size, sizeof(**graph));
  }

  for (size_t v = 0; v < hashes_size; v++) {
    for (size_t u = v + 1; u < hashes_size; u++) {
      if (hashes[v][u] > 0) {
        int uu = permutations[u];
        int vv = permutations[v];
        graph[uu][vv] = 1;
        graph[vv][uu] = 1;
      }
    }
  }

  int result = stoer_wagner(graph, graph_size);

  for (size_t j = 0; j < graph_size; j++) {
    free(graph[j]);
  }
  free(graph);
  for (size_t j = 0; j < hashes_size; j++) {
    free(hashes[j]);
  }
  free(hashes);
  free(permutations);
  string_array_free(nodes);

  return result;
}

int main(void) {
  // dot -Kdot -Tsvg ../../inputs/2023/day25/sample.dot > ../../inputs/2023/day25/sample.svg
  test_case(day25, part1, sample, 54);
  // dot -Kneato -Tsvg ../../inputs/2023/day25/data.dot > ../../inputs/2023/day25/data.svg
  test_case(day25, part1, data, 580800);

  return 0;
}
