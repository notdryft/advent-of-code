#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "string.h"

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

void debug_nodes(Array *nodes) {
  printf("Nodes[ ");
  for (size_t i = 0; i < nodes->size; i++) {
    Node *node = array_get(nodes, i);
    printf("{ %s: [ ", node->name);
    if (node->rules != nullptr) {
      for (size_t j = 0; j < node->rules->size; j++) {
        Rule *rule = array_get(node->rules, j);
        printf("{ %d %d '%c' %d %d \"%s\" }, ", rule->rule_type, rule->action, rule->category, rule->op, rule->quantity, rule->next);
      }
    }
    printf(" ] },");
  }
  printf(" ]\n");
}

void debug_ranges(Ranges *ranges) {
  printf("{ next = %s, x = [%d %d], m = [%d %d], a = [%d %d], s = [%d %d] }\n", ranges->next, ranges->x.min, ranges->x.max, ranges->m.min, ranges->m.max, ranges->a.min, ranges->a.max, ranges->s.min, ranges->s.max);
}

void debug_array_ranges(Array *array) {
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
  return nullptr;
}

bool accept_or_reject(Array *nodes, Node *node, int x, int m, int a, int s) {
  debug("new node name=%s\n", node->name);
  for (size_t i = 0; i < node->rules->size; i++) {
    Rule *rule = array_get(node->rules, i);
    debug("{ %d %d '%c' %d %d \"%s\" }\n", rule->rule_type, rule->action, rule->category, rule->op, rule->quantity, rule->next);
    if (rule->rule_type == AR) {
      if (rule->action == A) return true;
      else if (rule->action == R) return false;
    } else if (rule->rule_type == FOLLOW_NODE) {
      debug("next %s\n", rule->next);
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
      debug("next %s\n", rule->next);
      Node *next = find_node(nodes, rule->next);
      if (comparison_succeeded) return accept_or_reject(nodes, next, x, m, a, s);
    }
  }
  debug("end\n");
  return false;
}

int apply_op(enum Op op, int a, int b) {
  if (op == LT)      return min(a, b - 1);
  else if (op == GT) return max(a, b + 1);
  else if (op == LE) return min(a, b);
  else               return max(a, b);
}

void apply_rule(Rule *rule, Ranges *ranges, Ranges *copy) {
  if (rule->op == LT) {
    if (rule->category == 'x') {
      ranges->x.max = apply_op(rule->op, ranges->x.max, rule->quantity);
      copy->x.min = apply_op(GE, copy->x.min, rule->quantity);
    } else if (rule->category == 'm') {
      ranges->m.max = apply_op(rule->op, ranges->m.max, rule->quantity);
      copy->m.min  = apply_op(GE, copy->m.min, rule->quantity);
    } else if (rule->category == 'a') {
      ranges->a.max = apply_op(rule->op, ranges->a.max, rule->quantity);
      copy->a.min = apply_op(GE, copy->a.min, rule->quantity);
    } else if (rule->category == 's') {
      ranges->s.max = apply_op(rule->op, ranges->s.max, rule->quantity);
      copy->s.min = apply_op(GE, copy->s.min, rule->quantity);
    }
  } else if (rule->op == GT) {
    if (rule->category == 'x') {
      ranges->x.min = apply_op(rule->op, ranges->x.min, rule->quantity);
      copy->x.max = apply_op(LE, copy->x.max, rule->quantity);
    } else if (rule->category == 'm') {
      ranges->m.min = apply_op(rule->op, ranges->m.min, rule->quantity);
      copy->m.max = apply_op(LE, copy->m.max, rule->quantity);
    } else if (rule->category == 'a') {
      ranges->a.min = apply_op(rule->op, ranges->a.min, rule->quantity);
      copy->a.max = apply_op(LE, copy->a.max, rule->quantity);
    } else if (rule->category == 's') {
      ranges->s.min = apply_op(rule->op, ranges->s.min, rule->quantity);
      copy->s.max = apply_op(LE, copy->s.max, rule->quantity);
    }
  }
}

void reduce(Array *queue, Array *valid, Node *node, Ranges ranges) {
  for (size_t i = 0; i < node->rules->size; i++) {
    Ranges copy = ranges;
    Rule *rule = array_get(node->rules, i);
    debug("rule = %s\n", rule->next);
    if (rule->rule_type == AR) {
      debug("AR\n");
      if (rule->action == A) {
        array_push(valid, &ranges);
      }
      break;
    } else if (rule->rule_type == FOLLOW_NODE) {
      debug("FOLLOW_NODE\n");
      strncpy(ranges.next, rule->next, 4);
      array_push(queue, &ranges);
    } else if (rule->rule_type == COMPARE_AR) {
      debug("COMPARE_AR\n");
      apply_rule(rule, &ranges, &copy);
      debugf(debug_ranges, &copy);
      if (rule->action == A) array_push(valid, &ranges);
    } else if (rule->rule_type == COMPARE_NODE) {
      debug("COMPARE_NODE\n");
      apply_rule(rule, &ranges, &copy);
      strncpy(ranges.next, rule->next, 4);
      array_push(queue, &ranges);
    }
    ranges = copy;
  }
}

int part1(StringArray *lines) {
  int result = 0;

  Node *root = nullptr;
  Array *nodes = array_new(Node);

  for (size_t l = 0; l < lines->size; l++) {
    char *line = lines->items[l];
    size_t line_len = strlen(line);

    if (line_len == 0) {
      continue;
    }

    char name[4];
    memset(name, 0, sizeof(char) * 4);

    char str[40];
    memset(str, 0, sizeof(char) * 40);

    int x = -1, m = -1, a = -1, s = -1;

    if (sscanf(line, "%[a-z]{%[^}]}", name, str)) {
      debug("name: %s\n", name);
      debug("rules: %s\n", str);

      Node node = {};
      strncpy(node.name, name, 4);
      node.rules = array_new(Rule);

      StringArray *rules = string_split(str, ",");
      //debugf(string_array_print, rules);
      for (size_t i = 0; i < rules->size; i++) {
        char *rule_str = rules->items[i];
        if (rule_str[1] == '<' || rule_str[1] == '>') {
          StringArray *split = string_split(rule_str, ":");
          debugf(string_array_print, split);
          char *op_str = split->items[0];
          char *action_str = split->items[1];

          Rule rule = { .category = op_str[0] };
          if (op_str[1] == '<') {
            StringArray *op_split = string_split(op_str, "<");
            debugf(string_array_print, op_split);
            rule.category = op_split->items[0][0];
            rule.op = LT;
            rule.quantity = atoi(op_split->items[1]);
            string_array_free(op_split);
          } else {
            StringArray *op_split = string_split(op_str, ">");
            debugf(string_array_print, op_split);
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

          array_push(node.rules, &rule);

          string_array_free(split);
        } else if (rule_str[0] == 'A') {
          Rule rule = { .rule_type = AR, .action = A };
          array_push(node.rules, &rule);
        } else if (rule_str[0] == 'R') {
          Rule rule = { .rule_type = AR, .action = R };
          array_push(node.rules, &rule);
        } else { // just name
          Rule rule = { .rule_type = FOLLOW_NODE };
          strncpy(rule.next, rule_str, 3);
          array_push(node.rules, &rule);
        }
      }

      array_push(nodes, &node);

      string_array_free(rules);
    } else if (sscanf(line, "{x=%d,m=%d,a=%d,s=%d}", &x, &m, &a, &s)) {
      if (root == nullptr) {
        root = find_node(nodes, "in");
      }
      debug("%s\n", root->name);
      debug("x = %d, m = %d, a = %d, s = %d\n", x, m, a, s);
      debugf(debug_nodes, nodes);
      if (accept_or_reject(nodes, root, x, m, a, s)) {
        debug("x = %d, m = %d, a = %d, s = %d\n", x, m, a, s);
        debug("accepted\n");
        result += x + m + a + s;
      } else {
        debug("x = %d, m = %d, a = %d, s = %d\n", x, m, a, s);
        debug("rejected\n");
      }
    }
  }
  debugf(debug_nodes, nodes);

  return result;
}

unsigned long long part2(StringArray *lines) {
  Array *nodes = array_new(Node);

  for (size_t l = 0; l < lines->size; l++) {
    char *line = lines->items[l];
    size_t line_len = strlen(line);

    if (line_len == 0) {
      continue;
    }

    char name[4] = {};
    memset(name, 0, sizeof(char) * 4);

    char str[40];
    memset(str, 0, sizeof(char) * 40);

    if (sscanf(line, "%[a-z]{%[^}]}", name, str)) {
      debug("name: %s\n", name);
      debug("rules: %s\n", str);

      Node node = {};
      strncpy(node.name, name, 4);
      node.rules = array_new(Rule);

      StringArray *rules = string_split(str, ",");
      //debugf(string_array_print, rules);
      for (size_t i = 0; i < rules->size; i++) {
        char *rule_str = rules->items[i];
        if (rule_str[1] == '<' || rule_str[1] == '>') {
          StringArray *split = string_split(rule_str, ":");
          //debugf(string_array_print, split);
          char *op_str = split->items[0];
          char *action_str = split->items[1];

          Rule rule = { .category = op_str[0] };
          if (op_str[1] == '<') {
            StringArray *op_split = string_split(op_str, "<");
            //debugf(string_array_print, op_split);
            rule.category = op_split->items[0][0];
            rule.op = LT;
            rule.quantity = atoi(op_split->items[1]);
            string_array_free(op_split);
          } else {
            StringArray *op_split = string_split(op_str, ">");
            //debugf(string_array_print, op_split);
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

          array_push(node.rules, &rule);

          string_array_free(split);
        } else if (rule_str[0] == 'A') {
          Rule rule = { .rule_type = AR, .action = A };
          array_push(node.rules, &rule);
        } else if (rule_str[0] == 'R') {
          Rule rule = { .rule_type = AR, .action = R };
          array_push(node.rules, &rule);
        } else { // just name
          Rule rule = { .rule_type = FOLLOW_NODE };
          strncpy(rule.next, rule_str, 3);
          array_push(node.rules, &rule);
        }
      }

      array_push(nodes, &node);

      string_array_free(rules);
    }
  }
  debugf(debug_nodes, nodes);

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
  array_push(queue, &start);

  int i = 0;
  while (queue->size > 0) {
    Ranges *ranges = array_pop(queue);
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
    //debugf(debug_array_ranges, queue);
    //debugf(debug_array_ranges, valid);
    //debug("-------\n");
    //if (i==2) exit(1);
    i++;
  }

  unsigned long long result = 0;
  for (size_t i = 0; i < valid->size; i++) {
    Ranges *r = array_get(valid, i);
    unsigned long long x = (r->x.max - r->x.min + 1);
    unsigned long long m = (r->m.max - r->m.min + 1);
    unsigned long long a = (r->a.max - r->a.min + 1);
    unsigned long long s = (r->s.max - r->s.min + 1);
    unsigned long long score = x * m * a * s;
    debug("%llu\n", score);
    result += score;
  }

  free(nodes);

  return result;
}

int main(void) {
  test_case(day19, part1, sample, 19114);
  test_case(day19, part1, data, 367602);
  test_case(day19, part2, sample, 167409079868000);
  test_case(day19, part2, data, 125317461667458);
  //test_case(day19, part2, hard, 1);

  return 0;
}
