#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

#define hash(key) (key[0]-'0'+(key[1]-'0')*75+(key[2]-'0')*75*75)

constexpr size_t BOARD_SIZE = 75*75*75;

i64 board[BOARD_SIZE];

enum Gate {
  AND,
  OR,
  XOR,
  UNDEFINED
};

typedef struct {
  enum Gate gate;
  char lhs[4];
  char rhs[4];
  char target[4];
} Wire;

int wire_cmp(const void *a, const void *b) {
  const char *ta = ((Wire *) a)->target;
  const char *tb = ((Wire *) b)->target;
  return strncmp(ta, tb, 3);
}

void print_wires(Array *wires) {
  for (size_t i = 0; i < wires->size; i++) {
    Wire *wire = array_get(wires, i);
    size_t lh = hash(wire->lhs);
    size_t rh = hash(wire->rhs);
    size_t th = hash(wire->target);
    if (wire->gate == AND) {
      printf("%zu: %s (%lld) AND %s (%lld) -> %s (%lld)\n", i, wire->lhs, board[lh], wire->rhs, board[rh], wire->target, board[th]);
    } else if (wire->gate == OR) {
      printf("%zu: %s (%lld) OR %s (%lld) -> %s (%lld)\n", i, wire->lhs, board[lh], wire->rhs, board[rh], wire->target, board[th]);
    } else if (wire->gate == XOR) {
      printf("%zu: %s (%lld) XOR %s (%lld) -> %s (%lld)\n", i, wire->lhs, board[lh], wire->rhs, board[rh], wire->target, board[th]);
    } else if (wire->gate == UNDEFINED) {
      printf("%zu: ???", i);
    }
  }
}

int str_cmp(const void *a, const void *b) {
  const char *a_str = *(const char **) a;
  const char *b_str = *(const char **) b;
  return strcmp(a_str, b_str);
}

int reverse_str_cmp(const void *a, const void *b) {
  const char *a_str = *(const char **) a;
  const char *b_str = *(const char **) b;
  return strcmp(b_str, a_str);
}

i64 part1(StringArray *lines) {
  Array *wires = array_new(Wire);
  StringArray *outputs = string_array_new();

  for (size_t i = 0; i < BOARD_SIZE; i++) {
    board[i] = -1;
  }

  string_array_foreach (char *line, lines) {
    size_t line_len = strlen(line);

    char lhs[3], rhs[3], target[3];
    i64 value;

    if (sscanf(line, "%[^:]: %lld", target, &value) == 2) {
      debug("%s: %lld\n", target, value);
      board[hash(target)] = value;
    } else if (line_len > 0) {
      Wire wire = {};
      if (sscanf(line, "%s AND %s -> %s", lhs, rhs, target) == 3) {
        debug("%s AND %s -> %s\n", lhs, rhs, target);
        wire.gate = AND;
      } else if (sscanf(line, "%s OR %s -> %s", lhs, rhs, target) == 3) {
        debug("%s OR %s -> %s\n", lhs, rhs, target);
        wire.gate = OR;
      } else if (sscanf(line, "%s XOR %s -> %s", lhs, rhs, target) == 3) {
        debug("%s XOR %s -> %s\n", lhs, rhs, target);
        wire.gate = XOR;
      }
      memset(wire.lhs, 0, sizeof(char) * 4);
      strncpy(wire.lhs, lhs, 3);
      memset(wire.rhs, 0, sizeof(char) * 4);
      strncpy(wire.rhs, rhs, 3);
      memset(wire.target, 0, sizeof(char) * 4);
      strncpy(wire.target, target, 3);

      if (target[0] == 'z' && !string_array_contains(outputs, target)) {
        string_array_push(outputs, target);
      }

      array_push(wires, &wire);
    }
  }
  debugf(print_wires, wires);

  while (wires->size > 0) {
    Wire *wire = array_shift(wires);
    i64 lhsv = board[hash(wire->lhs)];
    i64 rhsv = board[hash(wire->rhs)];

    if (lhsv == -1 || rhsv == -1) {
      array_push(wires, wire);
      free(wire);
      continue;
    }

    i64 th = hash(wire->target);
    if (wire->gate == AND) {
      board[th] = lhsv & rhsv;
    } else if (wire->gate == OR) {
      board[th] = lhsv | rhsv;
    } else if (wire->gate == XOR) {
      board[th] = lhsv ^ rhsv;
    }

    free(wire);
  }

  qsort(outputs->items, outputs->size, sizeof(char *), reverse_str_cmp);
  debugf(string_array_print, outputs);

  i64 result = 0;
  for (size_t i = 0; i < outputs->size; i++) {
    char *z = string_array_get(outputs, i);
    debug("%s: %lld\n", z, board[hash(z)]);
    result = (result << 1) | board[hash(z)];
  }

  array_free(wires);
  string_array_free(outputs);

  return result;
}

char *find_wire(Array *wires, char *a, char *b, enum Gate gate) {
  for (size_t i = 0; i < wires->size; i++) {
    Wire *wire = array_get(wires, i);
    if (wire->gate == gate && (
      (strncmp(wire->lhs, a, 3) == 0 && strncmp(wire->rhs, b, 3) == 0) ||
      (strncmp(wire->lhs, b, 3) == 0 && strncmp(wire->rhs, a, 3) == 0) 
    )) {
      char *cpy = strdup(wire->target);
      return cpy;
    }
  }
  return nullptr;
}

void swap_wires(Array *wires, StringArray *swapped, char *a, char *b) {
  trace("swap_wires: %s and %s\n", a, b);
  trace("before:\n");
  tracef(print_wires, wires);

  for (size_t i = 0; i < wires->size; i++) {
    Wire *wire = array_get(wires, i);
    if (strncmp(wire->lhs, a, 3) == 0) {
      strncpy(wire->lhs, b, 3);
    } else if (strncmp(wire->lhs, b, 3) == 0) {
      strncpy(wire->lhs, a, 3);
    }
    if (strncmp(wire->rhs, a, 3) == 0) {
      strncpy(wire->rhs, b, 3);
    } else if (strncmp(wire->rhs, b, 3) == 0) {
      strncpy(wire->rhs, a, 3);
    }
    if (strncmp(wire->target, a, 3) == 0) {
      strncpy(wire->target, b, 3);
    } else if (strncmp(wire->target, b, 3) == 0) {
      strncpy(wire->target, a, 3);
    }
  }
  trace("after:\n");
  tracef(print_wires, wires);

  string_array_push(swapped, a);
  string_array_push(swapped, b);
}

i64 part2(StringArray *lines, char *expected) {
  Array *wires = array_new(Wire);
  StringArray *outputs = string_array_new();

  for (size_t i = 0; i < BOARD_SIZE; i++) {
    board[i] = -1;
  }

  string_array_foreach (char *line, lines) {
    size_t line_len = strlen(line);

    char lhs[3], rhs[3], target[3];
    i64 value;

    if (sscanf(line, "%[^:]: %lld", target, &value) == 2) {
      debug("%s: %lld\n", target, value);
      board[hash(target)] = value;
    } else if (line_len > 0) {
      Wire wire = {};
      if (sscanf(line, "%s AND %s -> %s", lhs, rhs, target) == 3) {
        debug("%s AND %s -> %s\n", lhs, rhs, target);
        wire.gate = AND;
      } else if (sscanf(line, "%s OR %s -> %s", lhs, rhs, target) == 3) {
        debug("%s OR %s -> %s\n", lhs, rhs, target);
        wire.gate = OR;
      } else if (sscanf(line, "%s XOR %s -> %s", lhs, rhs, target) == 3) {
        debug("%s XOR %s -> %s\n", lhs, rhs, target);
        wire.gate = XOR;
      }
      memset(wire.lhs, 0, sizeof(char) * 4);
      strncpy(wire.lhs, lhs, 3);
      memset(wire.rhs, 0, sizeof(char) * 4);
      strncpy(wire.rhs, rhs, 3);
      memset(wire.target, 0, sizeof(char) * 4);
      strncpy(wire.target, target, 3);

      if (target[0] == 'z' && !string_array_contains(outputs, target)) {
        string_array_push(outputs, target);
      }

      array_push(wires, &wire);
    }
  }
  debugf(print_wires, wires);

  StringArray *swapped = string_array_new();

  char *cin = nullptr;
  for (size_t i = 0; i < outputs->size - 1; i++) {
    debug("i: %zu/%zu\n", i, outputs->size - 1);

    // Full adder:
    // X ^ Y -> M
    // X & Y -> N
    // Cin & M -> R
    // Cin ^ M -> Z
    // R | N -> Cout

    char x[100] = {};
    sprintf(x, "x%02zu", i);
    char y[100] = {};
    sprintf(y, "y%02zu", i);

    char *m = find_wire(wires, x, y, XOR);
    char *n = find_wire(wires, x, y, AND);
    trace("> M: %s\n", m);
    trace("> N: %s\n", n);

    char *r = nullptr;
    char *z = nullptr;
    char *cout = nullptr;

    if (cin != nullptr) {
      trace("> Cin != nullptr: %s\n", cin);
      tracef(string_array_print, swapped);

      r = find_wire(wires, cin, m, AND);
      if (r == nullptr) {
        trace("> M == nullptr\n");
        swap_wires(wires, swapped, m, n);
        r = find_wire(wires, cin, m, AND);
      }
      
      z = find_wire(wires, cin, m, XOR);

      if (m != nullptr && m[0] == 'z') {
        trace("> M != nullptr: %s\n", m);
        swap_wires(wires, swapped, m, z);
      }

      if (n != nullptr && n[0] == 'z') {
        trace("> N != nullptr: %s\n", n);
        swap_wires(wires, swapped, n, z);
      }

      if (r != nullptr && r[0] == 'z') {
        trace("> R != nullptr: %s\n", r);
        swap_wires(wires, swapped, r, z);
      }

      cout = find_wire(wires, r, n, OR);
    }

    if (cout != nullptr && cout[0] == 'z' && strncmp(cout, "z45", 3) != 0) {
      swap_wires(wires, swapped, cout, z);
    }

    if (cin != nullptr) {
      free(cin);
      free(n);
      cin = cout;
    } else {
      cin = n;
    }

    free(m);
    free(r);
    free(z);
  }

  qsort(swapped->items, swapped->size, sizeof(char *), str_cmp);

  i64 result = swapped->size;
  char result_str[100] = {};
  for (size_t i = 0; i < swapped->size; i++) {
    if (i > 0) {
      size_t j = (i - 1) * 4 + 3;
      result_str[j] = ',';
      strncpy(result_str + j + 1, swapped->items[i], 3);
    } else {
      strncpy(result_str, swapped->items[i], 3);
    }
  }
  debug("result: %s\n", result_str);
 
  assert(result_str != nullptr);
  assert(strncmp(result_str, expected, strlen(expected)) == 0);
  assert(strlen(result_str) == strlen(expected));

  array_free(wires);
  string_array_free(swapped);
  string_array_free(outputs);

  return result;
}

int main(void) {
  test_case(day24, part1, sample1, 4);
  test_case(day24, part1, sample2, 2024);
  test_case(day24, part1, data, 53190357879014);
  test_case(day24, part2, data, 8, "bks,hnd,nrn,tdv,tjp,z09,z16,z23");

  return 0;
}
