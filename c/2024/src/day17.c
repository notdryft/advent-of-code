#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

enum OpCode {
  ADV, // 0
  BXL, // 1
  BST, // 2
  JNZ, // 3
  BXC, // 4
  OUT, // 5
  BDV, // 6
  CDV  // 7
};

typedef struct {
  i64 A;
  i64 B;
  i64 C;
} Registers;

i64 combo_value(Registers r, i64 operand) {
  if (0 <= operand && operand <= 3) {
    return operand;
  } else if (operand == 4) {
    return r.A;
  } else if (operand == 5) {
    return r.B;
  } else if (operand == 6) {
    return r.C;
  }
  return -1;
}

char *run(Array *program, Registers r) {
  Array *output = array_new(i64);
  debug("A: %lld, B: %lld, C: %lld\n", r.A, r.B, r.C);

  size_t p = 0;
  while (p < program->size) {
    i64 op_code = i64_array_get(program, p);
    i64 literal = i64_array_get(program, p + 1);

    if (op_code == ADV) {
      i64 operand = combo_value(r, literal);
      if (operand == -1) break;
      i64 d = pow(2, operand);
      r.A /= d;
      trace("ADV: A = %lld\n", r.A);
    } else if (op_code == BXL) {
      r.B ^= literal;
      trace("BXL: B = %lld\n", r.B);
    } else if (op_code == BST) {
      i64 operand = combo_value(r, literal);
      if (operand == -1) break;
      r.B = operand % 8;
      trace("BST: B = %lld\n", r.B);
    } else if (op_code == JNZ) {
      if (r.A > 0) {
        p = literal;
        trace("JNZ %lld\n", literal);
      } else if (r.A == 0) {
        trace("JNZ halt\n");
        break;
      }
    } else if (op_code == BXC) {
      r.B ^= r.C;
      trace("BXC: B = %lld\n", r.B);
    } else if (op_code == OUT) {
      i64 operand = combo_value(r, literal);
      if (operand == -1) break;
      i64 result = operand % 8;
      array_push_rval(output, result);
      trace("OUT %lld\n", result);
    } else if (op_code == BDV) {
      i64 operand = combo_value(r, literal);
      if (operand == -1) break;
      i64 d = pow(2, operand);
      r.B = r.A / d;
      trace("BDV: B = %lld\n", r.B);
    } else if (op_code == CDV) {
      i64 operand = combo_value(r, literal);
      if (operand == -1) break;
      i64 d = pow(2, operand);
      r.C = r.A / d;
      trace("CDV: C = %lld\n", r.C);
    }
    if (op_code != JNZ) {
      p += 2;
    }
    debug("A: %lld, B: %lld, C: %lld\n", r.A, r.B, r.C);
  }

  char *result = calloc(output->size * 2, sizeof(char));
  for (size_t i = 0; i < output->size; i++) {
    sprintf(result + i * 2, "%lld", i64_array_get(output, i));
    if (i < output->size - 1) sprintf(result + i * 2 + 1, ",");
  }

  array_free(output);

  return result;
}

i64 part1(StringArray *lines, char *expected) {
  char *result;

  Registers r = {};
  string_array_foreach (char *line, lines) {
    i64 n;
    char r_str[2] = {};
    char program_str[1000] = {};

    if (sscanf(line, "Register %[^:]: %lld", r_str, &n) == 2) {
      if (r_str[0] == 'A') r.A = n;
      else if (r_str[0] == 'B') r.B = n;
      else if (r_str[0] == 'C') r.C = n;
    } else if (sscanf(line, "Program: %s", program_str) == 1) {
      StringArray *split = string_split(program_str, ",");
      Array *program = string_atoll(split);
      debugf(i64_array_print, program);

      result = run(program, r);
      debug("result: %s\n", result);

      array_free(program);
      string_array_free(split);
    }
  }

  assert(result != nullptr);
  assert(strncmp(result, expected, strlen(expected)) == 0);
  assert(strlen(result) == strlen(expected));

  free(result);

  return 0;
}

void compute_rec(Array *program, char *program_str, size_t program_len, size_t j, i64 current, i64 *result) {
  if (j < program->size) {
    char partial[1000] = {};
    size_t k = 2 * j + 1;
    strncpy(partial, program_str + program_len - k, k);
    size_t partial_len = strlen(partial);

    current <<= 3;
    for (i64 a = 0; a < 8; a++) {
      i64 A = current + a;
      Registers r = { .A = A, .B = 0, .C = 0 };

      char *output = run(program, r);
      if (strncmp(partial, output, partial_len) == 0 && *result == 0) {
        if (partial_len == program_len) {
          *result = A;
        } else {
          compute_rec(program, program_str, program_len, j + 1, A, result);
        }
      }
      free(output);
    }
  }
}

void compute(Array *program, char *program_str, i64 *result) {
  size_t program_len = strlen(program_str);
  compute_rec(program, program_str, program_len, 0, 0, result);
}

i64 part2(StringArray *lines) {
  i64 result = 0;
 
  string_array_foreach (char *line, lines) {
    char program_str[1000] = {};
    if (sscanf(line, "Program: %s", program_str) == 1) {
      StringArray *split = string_split(program_str, ",");
      Array *program = string_atoll(split);
      debugf(i64_array_print, program);

      compute(program, program_str, &result);

      array_free(program);
      string_array_free(split);
    }
  }

  return result;
}

int main(void) {
  test_case(day17, part1, sample1, 0, "4,6,3,5,6,3,5,2,1,0");
  test_case(day17, part1, data, 0, "7,5,4,3,4,5,3,4,6");
  test_case(day17, part2, sample2, 117440);
  test_case(day17, part2, data, 164278899142333);

  return 0;
}
