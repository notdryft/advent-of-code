#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

typedef struct {
  i64 x;
  i64 y;
} Vec2;

i64 compute_part1(Vec2 a, Vec2 b, Vec2 prize) {
  for (i64 i = 1; i <= 100; i++) {
    for (i64 j = 1; j <= 100; j++) {
      i64 x = a.x * i + b.x * j;
      i64 y = a.y * i + b.y * j;
      if (prize.x == x && prize.y == y) {
        return i * 3 + j;
      }
    }
  }
  return 0;
}

i64 part1(StringArray *lines) {
  i64 result = 0;

  string_array_foreach (char *line, lines) {
    Vec2 a, b, prize;
    if (sscanf(line, "Button A: X+%lld, Y+%lld", &a.x, &a.y) == 2) {
      debug("A(%lld, %lld)\n", a.x, a.y);
    } else if (sscanf(line, "Button B: X+%lld, Y+%lld", &b.x, &b.y) == 2) {
      debug("B(%lld, %lld)\n", b.x, b.y);
    } else if (sscanf(line, "Prize: X=%lld, Y=%lld", &prize.x, &prize.y) == 2) {
      debug("Prize(%lld, %lld)\n", prize.x, prize.y);
      i64 tokens = compute_part1(a, b, prize);
      debug("Tokens: %lld\n\n", tokens);

      result += tokens;
    }
  }

  return result;
}

constexpr i64 FACTOR = 10000000000000;

i64 compute_part2(Vec2 a, Vec2 b, Vec2 prize) {
  i64 det = a.x * b.y - a.y * b.x;
  debug("det = %lld\n", det);
  if (det != 0) {
    i64 in = b.y * prize.x - b.x * prize.y;
    i64 jn = -a.y * prize.x + a.x * prize.y;
    if (in % det == 0 && jn % det == 0) {
      i64 i = in / det;
      i64 j = jn / det;
      debug("i = %lld, j = %lld\n", i, j);
      return i * 3 + j;
    }
  }
  return 0;
}

i64 part2(StringArray *lines) {
  i64 result = 0;

  string_array_foreach (char *line, lines) {
    Vec2 a, b, prize;
    if (sscanf(line, "Button A: X+%lld, Y+%lld", &a.x, &a.y) == 2) {
      debug("A(%lld, %lld)\n", a.x, a.y);
    } else if (sscanf(line, "Button B: X+%lld, Y+%lld", &b.x, &b.y) == 2) {
      debug("B(%lld, %lld)\n", b.x, b.y);
    } else if (sscanf(line, "Prize: X=%lld, Y=%lld", &prize.x, &prize.y) == 2) {
      prize.x += FACTOR;
      prize.y += FACTOR;
      debug("Prize(%lld, %lld)\n", prize.x, prize.y);
      i64 tokens = compute_part2(a, b, prize);
      debug("Tokens: %lld\n\n", tokens);

      result += tokens;
    }
  }

  return result;
}

int main(void) {
  test_case(day13, part1, sample, 480);
  test_case(day13, part1, data, 28138);
  test_case(day13, part2, sample, 875318608908);
  test_case(day13, part2, data, 108394825772874);

  return 0;
}
