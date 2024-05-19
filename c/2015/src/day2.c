#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

i64 part1(StringArray *lines) {
  i64 result = 0;

  string_array_foreach (char *line, lines) {
    i32 l, w, h;

    if (sscanf(line, "%dx%dx%d", &l, &w, &h)) {
      i32 s1 = l * w;
      i32 s2 = w * h;
      i32 s3 = h * l;
      i32 s = min(s1, min(s2, s3));
      i32 d = 2 * s1 + 2 * s2 + 2 * s3 + s;
      debug("%d %d %d = %d\n", l, w, h, d);
      
      result += d;
    }
  }

  return result;
}

int i32_cmp(const void *a, const void *b) {
  return *(i32 *) a - *(i32 *) b;
}

i64 part2(StringArray *lines) {
  i64 result = 0;

  string_array_foreach (char *line, lines) {
    i32 l, w, h;

    if (sscanf(line, "%dx%dx%d", &l, &w, &h)) {
      i32 ns[3] = { l, w, h };
      qsort(ns, 3, sizeof(i32), i32_cmp);
      debug("%d %d %d => %d %d %d\n", l, w, h, ns[0], ns[1], ns[2]);

      i32 d = 2 * ns[0] + 2 * ns[1] + l * w * h;
      debug("%d %d %d = %d\n", l, w, h, d);
      
      result += d;
    }
  }

  return result;
}

int main(void) {
  test_case(day2, part1, sample, 101);
  test_case(day2, part1, data, 1588178);
  test_case(day2, part2, sample, 48);
  test_case(day2, part2, data, 3783758);

  return 0;
}
