#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "math.h"
#include "string.h"

#define BUFFER_LENGTH 1024

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
  //printf("find_node(?, %s, %zu)\n", value, size);
  for (size_t i = 0; i < size; i++) {
    Node *node = nodes[i];
    if (strncmp(node->value, value, 3) == 0) {
      return node;
    }
  }
  return NULL;
}

Nodes *find_nodes(Node **nodes, char value, size_t size) {
  Nodes *result = (Nodes *) malloc(sizeof(Nodes));
  result->capacity = 1000;
  result->size = 0;

  result->items = (Node **) malloc(sizeof(Node *) * result->capacity);

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

int part1(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  char instructions[BUFFER_LENGTH] = {0};

  size_t nodes_capacity = 1000;
  size_t nodes_size = 0;
  Node **nodes = (Node **) malloc(sizeof(Node*) * nodes_capacity);

  char buffer[BUFFER_LENGTH] = {0};
  if (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';
    strncpy(instructions, buffer, buffer_len - 1);
  }
  fgets(buffer, BUFFER_LENGTH, fp);
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    char value[4];
    char left_str[4];
    char right_str[4];

    if (sscanf(buffer, "%3s = (%3s, %3s)", value, left_str, right_str)) {
      Node *node = find_node(nodes, value, nodes_size);
      if (node == NULL) {
        node = (Node *) malloc(sizeof(Node));
        strncpy(node->value, value, 4);
        nodes[nodes_size++] = node;
      }

      Node *left = find_node(nodes, left_str, nodes_size);
      if (left == NULL) {
        left = (Node *) malloc(sizeof(Node));
        strncpy(left->value, left_str, 4);
        nodes[nodes_size++] = left;
      }
      node->left = left;

      Node *right = find_node(nodes, right_str, nodes_size);
      if (right == NULL) {
        right = (Node *) malloc(sizeof(Node));
        strncpy(right->value, right_str, 4);
        printf("copied node value: %s\n", node->value);
        nodes[nodes_size++] = right;
      }
      node->right = right;
    }
  }
  fclose(fp);

  for (size_t i = 0; i < nodes_size; i++) {
    Node *n = nodes[i];
    printf("%s = (", n->value);
    if (n->left != NULL) {
      printf("%s", n->left->value);
    } else {
      printf("null");
    }
    if (n->right != NULL) {
      printf(", %s)\n", n->right->value);
    } else {
      printf(", null)\n");
    }
  }

  printf("nodes = %zu\n", nodes_size);
  Node *node = find_node(nodes, "AAA", nodes_size);

  size_t instructions_len = strlen(instructions);
  //printf("instructions(%zu) = %s\n", instructions_len, instructions);

  int steps = 0;
  bool stop = false;
  while (!stop) {
    int mod = steps % instructions_len;
    //printf("%s = (%s, %s) -> %c\n", node->value, node->left->value, node->right->value, instructions[mod]);
    if (instructions[mod] == 'L') {
      node = node->left;
    } else {
      node = node->right;
    }
    if (strncmp(node->value, "ZZZ", 3) == 0) {
      stop = true;
    }
    steps++;
  }

  printf("steps = %d\n", steps);

  free_nodes(nodes, nodes_size);

  return steps;
}

long long part2(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  char instructions[BUFFER_LENGTH] = {0};

  size_t nodes_capacity = 1000;
  size_t nodes_size = 0;
  Node **nodes = (Node **) malloc(sizeof(Node*) * nodes_capacity);

  char buffer[BUFFER_LENGTH] = {0};
  if (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';
    strncpy(instructions, buffer, buffer_len - 1);
  }
  fgets(buffer, BUFFER_LENGTH, fp);
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    char value[4];
    char left_str[4];
    char right_str[4];

    if (sscanf(buffer, "%3s = (%3s, %3s)", value, left_str, right_str)) {
      Node *node = find_node(nodes, value, nodes_size);
      if (node == NULL) {
        node = (Node *) malloc(sizeof(Node));
        strncpy(node->value, value, 4);
        nodes[nodes_size++] = node;
      }

      Node *left = find_node(nodes, left_str, nodes_size);
      if (left == NULL) {
        left = (Node *) malloc(sizeof(Node));
        strncpy(left->value, left_str, 4);
        nodes[nodes_size++] = left;
      }
      node->left = left;

      Node *right = find_node(nodes, right_str, nodes_size);
      if (right == NULL) {
        right = (Node *) malloc(sizeof(Node));
        strncpy(right->value, right_str, 4);
        printf("copied node value: %s\n", node->value);
        nodes[nodes_size++] = right;
      }
      node->right = right;
    }
  }
  fclose(fp);

  for (size_t i = 0; i < nodes_size; i++) {
    Node *n = nodes[i];
    printf("%s = (", n->value);
    if (n->left != NULL) {
      printf("%s", n->left->value);
    } else {
      printf("null");
    }
    if (n->right != NULL) {
      printf(", %s)\n", n->right->value);
    } else {
      printf(", null)\n");
    }
  }

  printf("nodes = %zu\n", nodes_size);
  Nodes *ghost_nodes = find_nodes(nodes, 'A', nodes_size);

  size_t instructions_len = strlen(instructions);
  //printf("instructions(%zu) = %s\n", instructions_len, instructions);

  int ghost_steps[ghost_nodes->size];
  memset(ghost_steps, 0, sizeof(int) * ghost_nodes->size);

  for (size_t i = 0; i < ghost_nodes->size; i++) {
    Node *node = ghost_nodes->items[i];

    int steps = 0;
    bool stop = false;
    while (!stop) {
      int mod = steps % instructions_len;
      //printf("%s = (%s, %s) -> %c\n", node->value, node->left->value, node->right->value, instructions[mod]);
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
    printf("steps = %d\n", steps);
  }
  
  long long result = ghost_steps[0];
  for (size_t i = 1; i < ghost_nodes->size; i++) {
    printf("lcm(%llu, %d) = ", result, ghost_steps[i]);
    result = lcm(result, ghost_steps[i]);
    printf("%llu\n", result);
  }

  printf("result = %llu\n", result);

  free_nodes(nodes, nodes_size);

  return result;
}

int main() {
  assert(part1("../../inputs/2023/day8/sample1") == 2);
  assert(part1("../../inputs/2023/day8/sample2") == 6);
  assert(part1("../../inputs/2023/day8/data") == 22411);
  assert(part2("../../inputs/2023/day8/sample3") == 6);
  assert(part2("../../inputs/2023/day8/data") == 11188774513823);

  return 0;
}
