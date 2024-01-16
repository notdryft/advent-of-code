#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commons.h"
#include "math.h"
#include "string.h"

typedef struct _Node {
 char value[4];
 struct _Node *left;
 struct _Node *right;
} Node;

typedef struct {
  size_t capacity;
  size_t size;
  Node **items;
} Nodes;

Node *find_node(Node **nodes, char *value, size_t size) {
  //debug("find_node(?, %s, %zu)\n", value, size);
  for (size_t i = 0; i < size; i++) {
    Node *node = nodes[i];
    if (strncmp(node->value, value, 3) == 0) {
      return node;
    }
  }
  return nullptr;
}

Nodes *find_nodes(Node **nodes, char value, size_t size) {
  Nodes *result = malloc(sizeof(*result));
  result->capacity = 1000;
  result->size = 0;

  result->items = calloc(result->capacity, sizeof(*result->items));

  for (size_t i = 0; i < size; i++) {
    Node *node = nodes[i];
    if (node->value[2] == value) {
      result->items[result->size++] = node;
    }
  }

  return result;
}

void free_nodes(Node **nodes, size_t nodes_size) {
  for (size_t i = 0; i < nodes_size; i++) {
    free(nodes[i]);
  }
  free(nodes);
}

int part1(StringArray *lines) {
  size_t nodes_capacity = 1000;
  size_t nodes_size = 0;
  Node **nodes = calloc(nodes_capacity, sizeof(*nodes));

  char *instructions = strdup(lines->items[0]);
  for (size_t l = 1; l < lines->size; l++) {
    char *line = lines->items[l];

    char value[4];
    char left_str[4];
    char right_str[4];

    if (sscanf(line, "%3s = (%3s, %3s)", value, left_str, right_str)) {
      Node *node = find_node(nodes, value, nodes_size);
      if (node == nullptr) {
        node = malloc(sizeof(*node));
        strncpy(node->value, value, 4);
        nodes[nodes_size++] = node;
      }

      Node *left = find_node(nodes, left_str, nodes_size);
      if (left == nullptr) {
        left = malloc(sizeof(*left));
        strncpy(left->value, left_str, 4);
        nodes[nodes_size++] = left;
      }
      node->left = left;

      Node *right = find_node(nodes, right_str, nodes_size);
      if (right == nullptr) {
        right = malloc(sizeof(*right));
        strncpy(right->value, right_str, 4);
        debug("copied node value: %s\n", node->value);
        nodes[nodes_size++] = right;
      }
      node->right = right;
    }
  }

#ifdef DEBUG
  for (size_t i = 0; i < nodes_size; i++) {
    Node *n = nodes[i];
    debug("%s = (", n->value);
    if (n->left != nullptr) {
      debug("%s", n->left->value);
    } else {
      debug("null");
    }
    if (n->right != nullptr) {
      debug(", %s)\n", n->right->value);
    } else {
      debug(", null)\n");
    }
  }
#endif

  debug("nodes = %zu\n", nodes_size);
  Node *node = find_node(nodes, "AAA", nodes_size);

  size_t instructions_len = strlen(instructions);
  debug("instructions(%zu) = %s\n", instructions_len, instructions);

  int result = 0;
  bool stop = false;
  while (!stop) {
    int mod = result % instructions_len;
    debug("%s = (%s, %s) -> %c\n", node->value, node->left->value, node->right->value, instructions[mod]);
    if (instructions[mod] == 'L') {
      node = node->left;
    } else {
      node = node->right;
    }
    if (strncmp(node->value, "ZZZ", 3) == 0) {
      stop = true;
    }
    result++;
  }

  free(instructions);
  free_nodes(nodes, nodes_size);

  return result;
}

long long part2(StringArray *lines) {
  size_t nodes_capacity = 1000;
  size_t nodes_size = 0;
  Node **nodes = calloc(nodes_capacity, sizeof(*nodes));

  char *instructions = strdup(lines->items[0]);
  for (size_t l = 0; l < lines->size; l++) {
    char *line = lines->items[l];

    char value[4];
    char left_str[4];
    char right_str[4];

    if (sscanf(line, "%3s = (%3s, %3s)", value, left_str, right_str)) {
      Node *node = find_node(nodes, value, nodes_size);
      if (node == nullptr) {
        node = malloc(sizeof(*node));
        strncpy(node->value, value, 4);
        nodes[nodes_size++] = node;
      }

      Node *left = find_node(nodes, left_str, nodes_size);
      if (left == nullptr) {
        left = malloc(sizeof(*left));
        strncpy(left->value, left_str, 4);
        nodes[nodes_size++] = left;
      }
      node->left = left;

      Node *right = find_node(nodes, right_str, nodes_size);
      if (right == nullptr) {
        right = malloc(sizeof(*right));
        strncpy(right->value, right_str, 4);
        debug("copied node value: %s\n", node->value);
        nodes[nodes_size++] = right;
      }
      node->right = right;
    }
  }

#ifdef DEBUG
  for (size_t i = 0; i < nodes_size; i++) {
    Node *n = nodes[i];
    debug("%s = (", n->value);
    if (n->left != nullptr) {
      debug("%s", n->left->value);
    } else {
      debug("null");
    }
    if (n->right != nullptr) {
      debug(", %s)\n", n->right->value);
    } else {
      debug(", null)\n");
    }
  }
#endif

  debug("nodes = %zu\n", nodes_size);
  Nodes *ghost_nodes = find_nodes(nodes, 'A', nodes_size);

  size_t instructions_len = strlen(instructions);
  debug("instructions(%zu) = %s\n", instructions_len, instructions);

  int *ghost_steps = calloc(ghost_nodes->size, sizeof(*ghost_steps));

  for (size_t i = 0; i < ghost_nodes->size; i++) {
    Node *node = ghost_nodes->items[i];

    int steps = 0;
    bool stop = false;
    while (!stop) {
      int mod = steps % instructions_len;
      debug("%s = (%s, %s) -> %c\n", node->value, node->left->value, node->right->value, instructions[mod]);
      if (instructions[mod] == 'L') {
        node = node->left;
      } else {
        node = node->right;
      }
      if (node->value[2] == 'Z') {
        stop = true;
      }
      steps++;
    }
    ghost_steps[i] = steps;
    debug("steps = %d\n", steps);
  }

  long long result = ghost_steps[0];
  for (size_t i = 1; i < ghost_nodes->size; i++) {
    debug("lcm(%llu, %d) = ", result, ghost_steps[i]);
    result = lcm(result, ghost_steps[i]);
    debug("%llu\n", result);
  }

  free(instructions);
  free(ghost_steps);
  free_nodes(nodes, nodes_size);

  return result;
}

int main(void) {
  test_case(day8, part1, sample1, 2);
  test_case(day8, part1, sample2, 6);
  test_case(day8, part1, data, 22411);
  test_case(day8, part2, sample3, 6);
  test_case(day8, part2, data, 11188774513823);

  return 0;
}
