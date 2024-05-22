#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

constexpr size_t BOARD_SIZE = 26 * 27 + 1;

#define hash(key) (i16)(((i16)(strlen(key)>1?(26+(key[1]-'a')+(key[0]-'a')*26):(key[0]-'a')))+1)
#define reverse_hash(hash) {hash>26?(hash-1)/26+'a'-1:(hash-1)+'a',hash>26?(hash-1)%26+'a':'\0','\0'}

void print_hashes() {
  for (char i = 'a'; i <= 'z'; i++) {
    char str[3] = {i,0,0};
    i16 h = hash(str);
    char r[3] = reverse_hash(h);
    printf("%s (%d) = %s\n", str, h, r);
  }
  for (char i = 'a'; i <= 'z'; i++) {
    for (char j = 'a'; j <= 'z'; j++) {
      char str[3] = {i,j,0};
      i16 h = hash(str);
      char r[3] = reverse_hash(h);
      printf("%s (%d) = %s\n", str, h, r);
    }
  }
}

enum Gate {
  ASSIGN,
  AND,
  OR,
  NOT,
  LSHIFT,
  RSHIFT,
  UNDEFINED
};

typedef struct {
  enum Gate gate;
  i16 lhs;
  i16 rhs;
  i16 target;
} Wire;

int wire_cmp(const void *a, const void *b) {
  return ((Wire *) a)->target - ((Wire *) b)->target;
}

Wire init_wire() {
  return (Wire) {
    .gate = UNDEFINED,
    .lhs = 0,
    .rhs = 0,
    .target = 0,
  };
}

void print_wires(Array *wires, u16 *board) {
  for (size_t i = 0; i < wires->size; i++) {
    Wire *wire = array_get(wires, i);
    char target[3] = reverse_hash(wire->target);
    if (wire->gate == ASSIGN) {
      if (wire->lhs > 0) {
        char lhs[3] = reverse_hash(wire->lhs);
        printf("%zu: %s (%hu) -> %s (%hu)\n", i, lhs, board[wire->lhs], target, board[wire->target]);
      } else {
        printf("%zu: %hd -> %s (%hu)\n", i, -wire->lhs, target, board[wire->target]);
      }
    } else if (wire->gate == AND) {
      char rhs[3] = reverse_hash(wire->rhs);
      if (wire->lhs > 0) {
        char lhs[3] = reverse_hash(wire->lhs);
        printf("%zu: %s (%hu) AND %s (%hu) -> %s (%hu)\n", i, lhs, board[wire->lhs], rhs, board[wire->rhs], target, board[wire->target]);
      } else {
        printf("%zu: %hd AND %s (%hu) -> %s (%hu)\n", i, -wire->lhs, rhs, board[wire->rhs], target, board[wire->target]);
      }
    } else if (wire->gate == OR) {
      char lhs[3] = reverse_hash(wire->lhs);
      char rhs[3] = reverse_hash(wire->rhs);
      printf("%zu: %s (%hu) OR %s (%hu) -> %s (%hu) \n", i, lhs, board[wire->lhs], rhs, board[wire->rhs], target, board[wire->target]);
    } else if (wire->gate == NOT) {
      char lhs[3] = reverse_hash(wire->lhs);
      printf("%zu: NOT %s (%hu) -> %s (%hu)\n", i, lhs, board[wire->lhs], target, board[wire->target]);
    } else if (wire->gate == LSHIFT) {
      char lhs[3] = reverse_hash(wire->lhs);
      printf("%zu: %s (%hu) LSHIFT %hd -> %s (%hu) \n", i, lhs, board[wire->lhs], -wire->rhs, target, board[wire->target]);
    } else if (wire->gate == RSHIFT) {
      char lhs[3] = reverse_hash(wire->lhs);
      printf("%zu: %s (%hu) RSHIFT %hd -> %s (%hu) \n", i, lhs, board[wire->lhs], -wire->rhs, target, board[wire->target]);
    } else if (wire->gate == UNDEFINED) {
      printf("%zu: ???", i);
    }
  }
}

void connect_wires(Array *wires, char *line) {
  debug("%s\t\t", line);

  i16 lhsv, rhsv;
  char lhs[3], rhs[3], target[3];

  Wire wire = init_wire();
  if (sscanf(line, "%hd -> %s", &lhsv, target) == 2) {
    debug("%hd -> %s\n", lhsv, target);
    wire.gate = ASSIGN;
    wire.lhs = -lhsv;
    wire.target = hash(target);
  } else if (sscanf(line, "%s -> %s", lhs, target) == 2) {
    debug("%s -> %s\n", lhs, target);
    wire.gate = ASSIGN;
    wire.lhs = hash(lhs);
    wire.target = hash(target);
  } else if (sscanf(line, "%hd AND %s -> %s", &lhsv, rhs, target) == 3) {
    debug("%hd AND %s -> %s\n", lhsv, rhs, target);
    wire.gate = AND;
    wire.lhs = -lhsv;
    wire.rhs = hash(rhs);
    wire.target = hash(target);
  } else if (sscanf(line, "%s AND %s -> %s", lhs, rhs, target) == 3) {
    debug("%s AND %s -> %s\n", lhs, rhs, target);
    wire.gate = AND;
    wire.lhs = hash(lhs);
    wire.rhs = hash(rhs);
    wire.target = hash(target);
  } else if (sscanf(line, "%s OR %s -> %s", lhs, rhs, target) == 3) {
    debug("%s OR %s -> %s\n", lhs, rhs, target);
    wire.gate = OR;
    wire.lhs = hash(lhs);
    wire.rhs = hash(rhs);
    wire.target = hash(target);
  } else if (sscanf(line, "%s LSHIFT %hd -> %s", lhs, &rhsv, target) == 3) {
    debug("%s LSHIFT %hd -> %s\n", lhs, rhsv, target);
    wire.gate = LSHIFT;
    wire.lhs = hash(lhs);
    wire.rhs = -rhsv;
    wire.target = hash(target);
  } else if (sscanf(line, "%s RSHIFT %hd -> %s", lhs, &rhsv, target) == 3) {
    debug("%s RSHIFT %hd -> %s\n", lhs, rhsv, target);
    wire.gate = RSHIFT;
    wire.lhs = hash(lhs);
    wire.rhs = -rhsv;
    wire.target = hash(target);
  } else if (sscanf(line, "NOT %s -> %s", lhs, target) == 2) {
    debug("NOT %s -> %s\n", lhs, target);
    wire.gate = NOT;
    wire.lhs = hash(lhs);
    wire.target = hash(target);
  }
  array_push_rval(wires, wire);
}

void connect_circuit(Array *wires, u16 *board, bool *triggered) {
  while (wires->size > 0) {
    trace(">>>>>>>>>\n");
    tracef(print_wires, wires, board);
    trace("<<<<<<<<<\n");

    Wire *wire = array_shift(wires);
    enum Gate gate = wire->gate;
    i16 lhs = wire->lhs;
    i16 rhs = wire->rhs;
    i16 target = wire->target;

    if (gate == ASSIGN) {
      if (lhs > 0) {
        if (triggered[lhs]) {
          board[target] = board[lhs];
          triggered[target] = true;
        } else {
          array_push(wires, wire);
        }
      } else {
        board[target] = -lhs;
        triggered[target] = true;
      }
    } else if (gate == AND) {
      if (lhs > 0) {
        if (triggered[lhs] && triggered[rhs]) {
          board[target] = board[lhs] & board[rhs];
          triggered[target] = true;
        } else {
          array_push(wires, wire);
        }
      } else {
        if (triggered[rhs]) {
          u16 lhsv = -lhs;
          board[target] = lhsv & board[rhs];
          triggered[target] = true;
        } else {
          array_push(wires, wire);
        }
      }
    } else if (gate == OR) {
      if (triggered[lhs] || triggered[rhs]) {
        board[target] = board[lhs] | board[rhs];
        triggered[target] = true;
      } else {
        array_push(wires, wire);
      }
    } else if (gate == NOT) {
      if (triggered[lhs]) {
        board[target] = ~board[lhs];
        triggered[target] = true;
      } else {
        array_push(wires, wire);
      }
    } else if (gate == LSHIFT) {
      u16 rhsv = -rhs;
      if (triggered[lhs]) {
        board[target] = board[lhs] << rhsv;
        triggered[target] = true;
      } else {
        array_push(wires, wire);
      }
    } else if (gate == RSHIFT) {
      u16 rhsv = -rhs;
      if (triggered[lhs]) {
        board[target] = board[lhs] >> rhsv;
        triggered[target] = true;
      } else {
        array_push(wires, wire);
      }
    }
    free(wire);
  }
}

i64 part1(StringArray *lines) {
  u16 *board = calloc(BOARD_SIZE, sizeof(*board));
  bool *triggered = calloc(BOARD_SIZE, sizeof(*triggered));
  Array *wires = array_new(Wire);

  string_array_foreach (char *line, lines) {
    connect_wires(wires, line);
  }
  array_sort(wires, wire_cmp); // FIXME bug when not sorted??
  debugf(print_wires, wires, board);

  connect_circuit(wires, board, triggered);
  i64 result = board[1];

  free(board);
  free(triggered);
  array_free(wires);

  //tracef(print_hashes);

  return result;
}

i64 part2(StringArray *lines) {
  u16 *board = calloc(BOARD_SIZE, sizeof(*board));
  bool *triggered = calloc(BOARD_SIZE, sizeof(*triggered));
  Array *wires = array_new(Wire);

  string_array_foreach (char *line, lines) {
    connect_wires(wires, line);
  }
  array_sort(wires, wire_cmp); // FIXME bug when not sorted??
  debugf(print_wires, wires, board);

  Array *second = array_dup(wires);
  connect_circuit(wires, board, triggered);
  i16 intermediate = board[1];

  memset(board, 0, BOARD_SIZE);
  memset(triggered, 0, BOARD_SIZE);

  i16 hb = hash("b");
  for (size_t i = 0; i < second->size; i++) {
    Wire *wire = array_get(second, i);
    if (wire->gate == ASSIGN && wire->target == hb) {
      wire->lhs = -intermediate;
      break;
    }
  }

  connect_circuit(second, board, triggered);
  i64 result = board[1];

  free(board);
  free(triggered);
  array_free(second);
  array_free(wires);

  //tracef(print_hashes);

  return result;
}

int main(void) {
  test_case(day7, part1, sample, 122);
  test_case(day7, part1, data, 956);
  test_case(day7, part2, data, 40149);

  return 0;
}
