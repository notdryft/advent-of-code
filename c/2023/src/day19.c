#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "string.h"

#define BUFFER_LENGTH 1024

enum RuleType {
  COMPARE_AR,
  COMPARE_NODE,
  AR,
  FOLLOW_NODE
};

enum Op {
  LT,
  GT,
  LE,
  GE
};

enum Action {
  A,
  R,
  NODE
};

typedef struct _Rule {
  enum RuleType rule_type;
  enum Action action;
  char category;
  enum Op op;
  int quantity;
  char next[4];
} Rule;

typedef struct _Node {
  char name[4];
  Array *rules;
} Node;

typedef struct {
  int min;
  int max;
} Range;

typedef struct {
  char next[4];
  Range x;
  Range m;
  Range a;
  Range s;
} Ranges;

void debug(Array *nodes) {
  printf("Nodes[ ");
  for (size_t i = 0; i < nodes->size; i++) {
    Node *node = array_get(nodes, i);
    printf("{ %s: [ ", node->name);
    if (node->rules != NULL) {
      for (size_t j = 0; j < node->rules->size; j++) {
        Rule *rule = array_get(node->rules, j);
        printf("{ %d %d '%c' %d %d \"%s\" }, ", rule->rule_type, rule->action, rule->category, rule->op, rule->quantity, rule->next);
      }
    }
    printf(" ] },");
  }
  printf(" ]\n");
}

void debug_ranges_solo(Ranges *ranges) {
  printf("{ next = %s, x = [%d %d], m = [%d %d], a = [%d %d], s = [%d %d] }\n", ranges->next, ranges->x.min, ranges->x.max, ranges->m.min, ranges->m.max, ranges->a.min, ranges->a.max, ranges->s.min, ranges->s.max);
}

void debug_ranges(Array *array) {
  printf("Ranges[ ");
  for (size_t i = 0; i < array->size; i++) {
    Ranges *ranges = array_get(array, i);
    printf("{ next = %s, x = [%d %d], m = [%d %d], a = [%d %d], s = [%d %d] }, ", ranges->next, ranges->x.min, ranges->x.max, ranges->m.min, ranges->m.max, ranges->a.min, ranges->a.max, ranges->s.min, ranges->s.max);
  }
  printf("]\n");
}

Node *find_node(Array *array, char *name) {
  for (size_t i = 0; i < array->size; i++) {
    Node *item = array_get(array, i);
    if (strcmp(name, item->name) == 0) {
      return item;
    }
  }
  return NULL;
}

bool accept_or_reject(Array *nodes, Node *node, int x, int m, int a, int s) {
  printf("new node name=%s\n", node->name);
  for (size_t i = 0; i < node->rules->size; i++) {
    printf("1\n");
    Rule *rule = array_get(node->rules, i);
    printf("2\n");
    printf("{ %d %d '%c' %d %d \"%s\" }\n", rule->rule_type, rule->action, rule->category, rule->op, rule->quantity, rule->next);
    if (rule->rule_type == AR) {
      if (rule->action == A) return true;
      else if (rule->action == R) return false;
    } else if (rule->rule_type == FOLLOW_NODE) {
      printf("next %s\n", rule->next);
      Node *next = find_node(nodes, rule->next);
      return accept_or_reject(nodes, next, x, m, a, s);
    } else if (rule->rule_type == COMPARE_AR) {
      bool comparison_succeeded = false;
      if (rule->op == LT) {
        if (rule->category == 'x') comparison_succeeded = x < rule->quantity;
        else if (rule->category == 'm') comparison_succeeded = m < rule->quantity;
        else if (rule->category == 'a') comparison_succeeded = a < rule->quantity;
        else if (rule->category == 's') comparison_succeeded = s < rule->quantity;
      } else {
        if (rule->category == 'x') comparison_succeeded = x > rule->quantity;
        else if (rule->category == 'm') comparison_succeeded = m > rule->quantity;
        else if (rule->category == 'a') comparison_succeeded = a > rule->quantity;
        else if (rule->category == 's') comparison_succeeded = s > rule->quantity;
      }
      if (rule->action == A && comparison_succeeded) return true;
      else if (rule->action == R && comparison_succeeded) return false;
    } else if (rule->rule_type == COMPARE_NODE) {
      bool comparison_succeeded = false;
      if (rule->op == LT) {
        if (rule->category == 'x') comparison_succeeded = x < rule->quantity;
        else if (rule->category == 'm') comparison_succeeded = m < rule->quantity;
        else if (rule->category == 'a') comparison_succeeded = a < rule->quantity;
        else if (rule->category == 's') comparison_succeeded = s < rule->quantity;
      } else {
        if (rule->category == 'x') comparison_succeeded = x > rule->quantity;
        else if (rule->category == 'm') comparison_succeeded = m > rule->quantity;
        else if (rule->category == 'a') comparison_succeeded = a > rule->quantity;
        else if (rule->category == 's') comparison_succeeded = s > rule->quantity;
      }
      printf("next %s\n", rule->next);
      Node *next = find_node(nodes, rule->next);
      if (comparison_succeeded) return accept_or_reject(nodes, next, x, m, a, s);
    }
  }
  printf("end\n");
  return false;
}

int max(int a, int b) {
  return (a > b) ? a : b;
}

int min(int a, int b) {
  return (a < b) ? a : b;
}

int range(enum Op op, int a, int b) {
  if (op == LT)      return min(a, b - 1);
  else if (op == GT) return max(a, b + 1);
  else if (op == LE) return min(a, b);
  else               return max(a, b);
}

void do_something(Rule *rule, Ranges *ranges, Ranges *copy) {
  if (rule->op == LT) {
    if (rule->category == 'x') {
      ranges->x.max = range(rule->op, ranges->x.max, rule->quantity);
      copy->x.min = range(GE, copy->x.min, rule->quantity);
    } else if (rule->category == 'm') {
      ranges->m.max = range(rule->op, ranges->m.max, rule->quantity);
      copy->m.min  = range(GE, copy->m.min, rule->quantity);
    } else if (rule->category == 'a') {
      ranges->a.max = range(rule->op, ranges->a.max, rule->quantity);
      copy->a.min = range(GE, copy->a.min, rule->quantity);
    } else if (rule->category == 's') {
      ranges->s.max = range(rule->op, ranges->s.max, rule->quantity);
      copy->s.min = range(GE, copy->s.min, rule->quantity);
    }
  } else if (rule->op == GT) {
    if (rule->category == 'x') {
      ranges->x.min = range(rule->op, ranges->x.min, rule->quantity);
      copy->x.max = range(LE, copy->x.max, rule->quantity);
    } else if (rule->category == 'm') {
      ranges->m.min = range(rule->op, ranges->m.min, rule->quantity);
      copy->m.max = range(LE, copy->m.max, rule->quantity);
    } else if (rule->category == 'a') {
      ranges->a.min = range(rule->op, ranges->a.min, rule->quantity);
      copy->a.max = range(LE, copy->a.max, rule->quantity);
    } else if (rule->category == 's') {
      ranges->s.min = range(rule->op, ranges->s.min, rule->quantity);
      copy->s.max = range(LE, copy->s.max, rule->quantity);
    }
  }
}

void reduce(Array *queue, Array *valid, Node *node, Ranges ranges) {
  for (size_t i = 0; i < node->rules->size; i++) {
    Ranges copy = ranges;
    Rule *rule = array_get(node->rules, i);
    printf("rule = %s\n", rule->next);
    if (rule->rule_type == AR) {
      printf("AR\n");
      if (rule->action == A) {
        array_push(valid, ranges);
      }
      break;
    } else if (rule->rule_type == FOLLOW_NODE) {
      printf("FOLLOW_NODE\n");
      strncpy(ranges.next, rule->next, 4);
      array_push(queue, ranges);
    } else if (rule->rule_type == COMPARE_AR) {
      printf("COMPARE_AR\n");
      do_something(rule, &ranges, &copy);
      debug_ranges_solo(&copy);
      if (rule->action == A) array_push(valid, ranges);
    } else if (rule->rule_type == COMPARE_NODE) {
      printf("COMPARE_NODE\n");
      do_something(rule, &ranges, &copy);
      strncpy(ranges.next, rule->next, 4);
      array_push(queue, ranges);
    }
    ranges = copy;
  }
}

int part1(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  int sum = 0;

  Node *root = NULL;
  Array *nodes = array_new(Node);

  char buffer[BUFFER_LENGTH] = {};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';
    if (buffer[0] == '\0') {
      continue;
    }

    char name[4];
    memset(name, 0, sizeof(char) * 4);

    char str[40];
    memset(str, 0, sizeof(char) * 40);

    int x = -1, m = -1, a = -1, s = -1;

    if (sscanf(buffer, "%[a-z]{%[^}]}", name, str)) {
      //printf("name: %s\n", name);
      //printf("rules: %s\n", str);

      Node node = {};
      strncpy(node.name, name, 4);
      node.rules = array_new(Rule);

      StringArray *rules = string_split(str, ",");
      //string_array_print(rules);
      for (size_t i = 0; i < rules->size; i++) {
        char *rule_str = rules->items[i];
        if (rule_str[1] == '<' || rule_str[1] == '>') {
          StringArray *split = string_split(rule_str, ":");
          string_array_print(split);
          char *op_str = split->items[0];
          char *action_str = split->items[1];

          Rule rule = { .category = op_str[0] };
          if (op_str[1] == '<') {
            StringArray *op_split = string_split(op_str, "<");
            string_array_print(op_split);
            rule.category = op_split->items[0][0];
            rule.op = LT;
            rule.quantity = atoi(op_split->items[1]);
            string_array_free(op_split);
          } else {
            StringArray *op_split = string_split(op_str, ">");
            string_array_print(op_split);
            rule.category = op_split->items[0][0];
            rule.op = GT;
            rule.quantity = atoi(op_split->items[1]);
            string_array_free(op_split);
          }

          if (action_str[0] == 'A') {
            rule.action = A;
            rule.rule_type = COMPARE_AR;
          } else if (action_str[0] == 'R') {
            rule.action = R;
            rule.rule_type = COMPARE_AR;
          } else {
            rule.action = NODE;
            rule.rule_type = COMPARE_NODE;
            strncpy(rule.next, action_str, 3);
          }

          array_push(node.rules, rule);

          string_array_free(split);
        } else if (rule_str[0] == 'A') {
          Rule rule = { .rule_type = AR, .action = A };
          array_push(node.rules, rule);
        } else if (rule_str[0] == 'R') {
          Rule rule = { .rule_type = AR, .action = R };
          array_push(node.rules, rule);
        } else { // just name
          Rule rule = { .rule_type = FOLLOW_NODE };
          strncpy(rule.next, rule_str, 3);
          array_push(node.rules, rule);
        }
      }
      
      array_push(nodes, node);

      string_array_free(rules);
    } else if (sscanf(buffer, "{x=%d,m=%d,a=%d,s=%d}", &x, &m, &a, &s)) {
      if (root == NULL) {
        root = find_node(nodes, "in");
      }
      printf("%s\n", root->name);
      printf("x = %d, m = %d, a = %d, s = %d\n", x, m, a, s);
      debug(nodes);
      if (accept_or_reject(nodes, root, x, m, a, s)) {
        printf("x = %d, m = %d, a = %d, s = %d\n", x, m, a, s);
        printf("accepted\n");
        sum += x + m + a + s;
      } else {
        printf("x = %d, m = %d, a = %d, s = %d\n", x, m, a, s);
        printf("rejected\n");
      }
    }
  }

  debug(nodes);

  fclose(fp);

  printf("sum = %d\n", sum);

  return sum;
}

unsigned long long part2(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  Array *nodes = array_new(Node);

  char buffer[BUFFER_LENGTH] = {};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';
    if (buffer[0] == '\0') {
      continue;
    }

    char name[4];
    memset(name, 0, sizeof(char) * 4);

    char str[40];
    memset(str, 0, sizeof(char) * 40);

    if (sscanf(buffer, "%[a-z]{%[^}]}", name, str)) {
      //printf("name: %s\n", name);
      //printf("rules: %s\n", str);

      Node node = {};
      strncpy(node.name, name, 4);
      node.rules = array_new(Rule);

      StringArray *rules = string_split(str, ",");
      //string_array_print(rules);
      for (size_t i = 0; i < rules->size; i++) {
        char *rule_str = rules->items[i];
        if (rule_str[1] == '<' || rule_str[1] == '>') {
          StringArray *split = string_split(rule_str, ":");
          //string_array_print(split);
          char *op_str = split->items[0];
          char *action_str = split->items[1];

          Rule rule = { .category = op_str[0] };
          if (op_str[1] == '<') {
            StringArray *op_split = string_split(op_str, "<");
            //string_array_print(op_split);
            rule.category = op_split->items[0][0];
            rule.op = LT;
            rule.quantity = atoi(op_split->items[1]);
            string_array_free(op_split);
          } else {
            StringArray *op_split = string_split(op_str, ">");
            //string_array_print(op_split);
            rule.category = op_split->items[0][0];
            rule.op = GT;
            rule.quantity = atoi(op_split->items[1]);
            string_array_free(op_split);
          }

          if (action_str[0] == 'A') {
            rule.action = A;
            rule.rule_type = COMPARE_AR;
          } else if (action_str[0] == 'R') {
            rule.action = R;
            rule.rule_type = COMPARE_AR;
          } else {
            rule.action = NODE;
            rule.rule_type = COMPARE_NODE;
            strncpy(rule.next, action_str, 3);
          }

          array_push(node.rules, rule);

          string_array_free(split);
        } else if (rule_str[0] == 'A') {
          Rule rule = { .rule_type = AR, .action = A };
          array_push(node.rules, rule);
        } else if (rule_str[0] == 'R') {
          Rule rule = { .rule_type = AR, .action = R };
          array_push(node.rules, rule);
        } else { // just name
          Rule rule = { .rule_type = FOLLOW_NODE };
          strncpy(rule.next, rule_str, 3);
          array_push(node.rules, rule);
        }
      }
      
      array_push(nodes, node);

      string_array_free(rules);
    }
  }
  fclose(fp);

  debug(nodes);

  Ranges start = {
    .next = "in",
    .x = {
      .min = 1,
      .max = 4000
    },
    .m = {
      .min = 1,
      .max = 4000
    },
    .a = {
      .min = 1,
      .max = 4000
    },
    .s = {
      .min = 1,
      .max = 4000
    },
  };

  Array *queue = array_new(Ranges);
  Array *valid = array_new(Ranges);
  array_push(queue, start);

  int i = 0;
  while (queue->size > 0) {
    Ranges *ranges = (Ranges *) array_pop(queue);
    if (
      ranges->x.max < ranges->x.min ||
      ranges->m.max < ranges->m.min ||
      ranges->a.max < ranges->a.min ||
      ranges->s.max < ranges->s.min
    ) {
      continue;
    }
    Node *node = find_node(nodes, ranges->next);
    reduce(queue, valid, node, *ranges);
    //debug_ranges(queue);
    debug_ranges(valid);
    printf("-------\n");
    //if (i==2) exit(1);
    i++;
  }

  unsigned long long sum = 0;
  for (size_t i = 0; i < valid->size; i++) {
    Ranges *r = array_get(valid, i);
    unsigned long long x = (r->x.max - r->x.min + 1);
    unsigned long long m = (r->m.max - r->m.min + 1);
    unsigned long long a = (r->a.max - r->a.min + 1);
    unsigned long long s = (r->s.max - r->s.min + 1);
    unsigned long long score = x * m * a * s;
    printf("%llu\n", score);
    sum += score;
  }
  printf("sum = %llu\n", sum);

  free(nodes);

  return sum;
}

int main(void) {
  assert(part1("../../inputs/2023/day19/sample") == 19114);
  assert(part1("../../inputs/2023/day19/data") == 367602);
  assert(part2("../../inputs/2023/day19/sample") == 167409079868000);
  assert(part2("../../inputs/2023/day19/data") == 125317461667458);

  return 0;
}
