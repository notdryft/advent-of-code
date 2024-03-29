#include <limits.h>
#include <math.h>
#include <quadmath.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "string.h"

typedef struct {
  i128 x;
  i128 y;
  i128 z;
} Vec3;

Vec3 sub(Vec3 a, Vec3 b) {
  return (Vec3) { a.x - b.x, a.y - b.y, a.z - b.z };
}

Vec3 cross(Vec3 a, Vec3 b) {
  return (Vec3) {
    a.y * b.z - a.z * b.y,
    a.z * b.x - a.x * b.z,
    a.x * b.y - a.y * b.x
  };
}

void cross_matrix(Vec3 v, i128 M[3][3]) {
  M[0][0] =    0; M[0][1] = -v.z; M[0][2] =  v.y;
  M[1][0] =  v.z; M[1][1] =    0; M[1][2] = -v.x;
  M[2][0] = -v.y; M[2][1] =  v.x; M[2][2] =    0;
}

void merge(i128 M[6][6], i128 Mx[3][3], size_t p, size_t q) {
  for (size_t j = 0; j < 3; j++) {
    for (size_t i = 0; i < 3; i++) {
      M[q + j][p + i] = Mx[j][i];
    }
  }
}

void minor(i128 M[6][6], i128 m[5][5], size_t p, size_t q) {
  size_t jj = 0;
  for (size_t j = 0; j < 6; j++) {
    if (j == q) continue;
    size_t ii = 0;
    for (size_t i = 0; i < 6; i++) {
      if (i == p) continue;
      m[jj][ii++] = M[j][i];
    }
    jj++;
  }
}

f128 det5(i128 m[5][5]) {
  f128 det = 1;
  f128 total = 1;

  f128 copy[5][5];
  for (size_t j = 0; j < 5; j++) {
    for (size_t i = 0; i < 5; i++) {
      copy[j][i] = m[j][i];
    }
  }

  for (size_t i = 0; i < 5; i++) {
    size_t index = i;
    while (index < 5 && copy[index][i] == 0) {
      index++;
    }
    if (index == 5) {
      continue;
    }
    if (index != i) {
      for (size_t j = 0; j < 5; j++) {
        f128 t = copy[index][j];
        copy[index][j] = copy[i][j];
        copy[i][j] = t;
      }
      det = -det;
    }

    f128 tmp[5];
    for (size_t j = 0; j < 5; j++) {
      tmp[j] = copy[i][j];
    }
    for (size_t j = i + 1; j < 5; j++) {
      f128 a = tmp[i];
      f128 b = copy[j][i];
      for (size_t k = 0; k < 5; k++) {
        copy[j][k] = a * copy[j][k] - b * tmp[k];
      }
      total = total * a;
    }
  }

  for (size_t i = 0; i < 5; i++) {
    det = det * copy[i][i];
  }

  return det / total;
}

void cofactor(i128 M[6][6], f128 C[6][6]) {
  for (size_t j = 0; j < 6; j++) {
    for (size_t i = 0; i < 6; i++) {
      i128 m[5][5];
      minor(M, m, i, j);
      C[j][i] = det5(m) * powq(-1, i + j);
    }
  }
}

void transpose(f128 M[6][6]) {
  for (size_t j = 0; j < 6; j++) {
    for (size_t i = j + 1; i < 6; i++) {
      f128 tmp = M[j][i];
      M[j][i] = M[i][j];
      M[i][j] = tmp;
    }
  }
}

f128 determinant(i128 M[6][6]) {
  f128 det = 1;
  f128 total = 1;

  f128 copy[6][6];
  for (size_t j = 0; j < 6; j++) {
    for (size_t i = 0; i < 6; i++) {
      copy[j][i] = M[j][i];
    }
  }

  for (size_t i = 0; i < 6; i++) {
    size_t index = i;
    while (index < 6 && copy[index][i] == 0) {
      index++;
    }
    if (index == 6) {
      continue;
    }
    if (index != i) {
      for (size_t j = 0; j < 6; j++) {
        f128 t = copy[index][j];
        copy[index][j] = copy[i][j];
        copy[i][j] = t;
      }
      det = -det;
    }

    f128 tmp[6];
    for (size_t j = 0; j < 6; j++) {
      tmp[j] = copy[i][j];
    }
    for (size_t j = i + 1; j < 6; j++) {
      f128 a = tmp[i];
      f128 b = copy[j][i];
      for (size_t k = 0; k < 6; k++) {
        copy[j][k] = a * copy[j][k] - b * tmp[k];
      }
      total = total * a;
    }
  }

  for (size_t i = 0; i < 6; i++) {
    det = det * copy[i][i];
  }

  return det / total;
}

void debugi(i128 M[6][6]) {
  for (size_t j = 0; j < 6; j++) {
    for (size_t i = 0; i < 6; i++) {
      printf("%20lld", (ll) M[j][i]);
    }
    printf("\n");
  }
}

void debugq(f128 M[6][6]) {
  for (size_t j = 0; j < 6; j++) {
    for (size_t i = 0; i < 6; i++) {
      printf("%20.5Lf", (ld) M[j][i]);
    }
    printf("\n");
  }
}

void inverse(i128 M[6][6], f128 I[6][6]) {
  f128 d = determinant(M);
  debug("d = %lld\n", (ll) d);

  f128 A[6][6];
  cofactor(M, A);
  debug("A:\n"); debugf(debugq, A);

  transpose(A);
  debug("A^T:\n"); debugf(debugq, A);

  for (size_t j = 0; j < 6; j++) {
    for (size_t i = 0; i < 6; i++) {
      I[j][i] = (f128) A[j][i] / d;
    }
  }
}

void multiply(f128 M[6][6], i128 v[6], f128 r[6]) {
  for (size_t j = 0; j < 6; j++) {
    f128 rj = 0;
    for (size_t i = 0; i < 6; i++) {
      rj += M[j][i] * v[i];
    }
    r[j] = rj;
  }
}

typedef struct {
  Vec3 p;
  Vec3 v;
} Hailstone;

typedef struct {
  f128 x;
  f128 y;
  bool futureA;
  bool futureB;
} Intersection;

// https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection
bool intersects(Hailstone *a, Hailstone *b, Intersection *p) {
  // L1 being defined by two distinct points (x1, y1) and (x2, y2)
  i128 x1 = a->p.x;
  i128 y1 = a->p.y;
  i128 x2 = a->p.x + a->v.x;
  i128 y2 = a->p.y + a->v.y;
  // L2 being defined by two distinct points (x3, y3) and (x4, y4)
  i128 x3 = b->p.x;
  i128 y3 = b->p.y;
  i128 x4 = b->p.x + b->v.x;
  i128 y4 = b->p.y + b->v.y;

  i128 d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
  if (d == 0) {
    return false;
  }

  i128 pxn = (x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4);
  i128 pyn = (x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4);

  p->x = (f128) pxn / d;
  p->y = (f128) pyn / d;
  p->futureA = (p->x > x1) == (x2 > x1);
  p->futureB = (p->x > x3) == (x4 > x3);

  return true;
}

int part1(StringArray *lines, i128 min, i128 max) {
  Array *hailstones = array_new(Hailstone);

  string_array_foreach (char *line, lines) {
    StringArray *at_split = string_split(line, " @ ");
    StringArray *positions = string_split(at_split->items[0], ", ");
    i128 px = atoll(positions->items[0]);
    i128 py = atoll(positions->items[1]);
    i128 pz = atoll(positions->items[2]);
    StringArray *velocities = string_split(at_split->items[1], ", ");
    i128 vx = atoll(velocities->items[0]);
    i128 vy = atoll(velocities->items[1]);
    i128 vz = atoll(velocities->items[2]);

    array_push(hailstones, &(Hailstone) { { px, py, pz }, { vx, vy, vz } });

    string_array_free(velocities);
    string_array_free(positions);
    string_array_free(at_split);
  }

  int result = 0;
  for (size_t i = 0; i < hailstones->size - 1; i++) {
    Hailstone *a = array_get(hailstones, i);
    for (size_t j = i + 1; j < hailstones->size; j++) {
      Hailstone *b = array_get(hailstones, j);
      debug("Hailstone A: %lld, %lld, %lld @ %lld, %lld, %lld\n", (ll) a->p.x, (ll) a->p.y, (ll) a->p.z, (ll) a->v.x, (ll) a->v.y, (ll) a->v.z);
      debug("Hailstone B: %lld, %lld, %lld @ %lld, %lld, %lld\n", (ll) b->p.x, (ll) b->p.y, (ll) b->p.z, (ll) b->v.x, (ll) b->v.y, (ll) b->v.z);

      Intersection p;
      if (intersects(a, b, &p)) {
        if (p.futureA) {
          if (p.futureB) {
            if (min <= p.x && p.x <= max && min <= p.y && p.y <= max) {
              debug("Hailstones' paths will cross inside the test area (at x=%.3Lf, y=%.3Lf).\n",  (ld) p.x, (ld) p.y);
              result += 1;
            } else {
              debug("Hailstones' paths will cross outside the test area (at x=%.3Lf, y=%.3Lf).\n", (ld) p.x, (ld) p.y);
            }
          } else {
            debug("Hailstones' paths crossed in the past for hailstone B.\n");
          }
        } else if (p.futureB) {
          debug("Hailstones' paths crossed in the past for hailstone A.\n");
        } else if (!p.futureA && !p.futureB) {
          debug("Hailstones' paths crossed in the past for both hailstones.\n");
        }
      } else {
        debug("Hailstones' paths are parallel; they never intersect.\n");
      }
      debug("\n");
    }
  }

  array_free(hailstones);

  return result;
}

i128 part2(StringArray *lines) {
  Array *hailstones = array_new(Hailstone);

  string_array_foreach (char *line, lines) {
    StringArray *at_split = string_split(line, " @ ");
    StringArray *positions = string_split(at_split->items[0], ", ");
    i128 px = atoll(positions->items[0]);
    i128 py = atoll(positions->items[1]);
    i128 pz = atoll(positions->items[2]);
    StringArray *velocities = string_split(at_split->items[1], ", ");
    i128 vx = atoll(velocities->items[0]);
    i128 vy = atoll(velocities->items[1]);
    i128 vz = atoll(velocities->items[2]);

    array_push(hailstones, &(Hailstone) { { px, py, pz }, { vx, vy, vz } });

    string_array_free(velocities);
    string_array_free(positions);
    string_array_free(at_split);
  }

  Hailstone *h0 = array_get(hailstones, 0);
  Hailstone *h1 = array_get(hailstones, 1);
  Hailstone *h2 = array_get(hailstones, 2);

  i128 M0[3][3]; // ┌       ┐
  i128 M1[3][3]; // | M0 M2 |
  i128 M2[3][3]; // | M1 M3 |
  i128 M3[3][3]; // └       ┘

  cross_matrix(sub(h0->v, h1->v), M0);
  cross_matrix(sub(h0->v, h2->v), M1);
  cross_matrix(sub(h1->p, h0->p), M2);
  cross_matrix(sub(h2->p, h0->p), M3);

  i128 M[6][6];
  merge(M, M0, 0, 0);
  merge(M, M1, 0, 3);
  merge(M, M2, 3, 0);
  merge(M, M3, 3, 3);
  debug("M:\n"); debugf(debugi, M);

  f128 I[6][6];
  inverse(M, I);
  debug("I:\n"); debugf(debugq, I);

  i128 v[6];
  Vec3 v0 = sub(cross(h1->p, h1->v), cross(h0->p, h0->v));
  Vec3 v1 = sub(cross(h2->p, h2->v), cross(h0->p, h0->v));
  v[0] = v0.x; v[1] = v0.y; v[2] = v0.z; v[3] = v1.x; v[4] = v1.y; v[5] = v1.z;
  debug("rhs: %lld %lld %lld %lld %lld %lld\n", (ll) v[0], (ll) v[1], (ll) v[2], (ll) v[3], (ll) v[4], (ll) v[5]);

  f128 r[6];
  multiply(I, v, r);
  for (size_t i = 0; i < 6; i++) {
    r[i] = roundq(r[i]);
  }
  debug("r: %.Lf %.Lf %.Lf %.Lf %.Lf %.Lf\n", (ld) r[0], (ld) r[1], (ld) r[2], (ld) r[3], (ld) r[4], (ld) r[5]);

  i128 result = r[0] + r[1] + r[2];

  array_free(hailstones);

  return result;
}

int main(void) {
  test_case(day24, part1, sample, 2, 7, 27);
  test_case(day24, part1, data, 18184, 200000000000000, 400000000000000);
  test_case(day24, part2, sample, 47);
  test_case(day24, part2, data, 557789988450159);

  return 0;
}
