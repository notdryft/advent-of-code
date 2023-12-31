#include <assert.h>
#include <limits.h>
#include <math.h>
#include <quadmath.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "string.h"

#define BUFFER_LENGTH 1024

typedef long long ll;
typedef long double ld;

typedef struct {
  __int128 x;
  __int128 y;
  __int128 z;
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

void cross_matrix(Vec3 v, __int128 M[3][3]) {
  M[0][0] =    0; M[0][1] = -v.z; M[0][2] =  v.y;
  M[1][0] =  v.z; M[1][1] =    0; M[1][2] = -v.x;
  M[2][0] = -v.y; M[2][1] =  v.x; M[2][2] =    0;
}

void merge(__int128 M[6][6], __int128 Mx[3][3], size_t p, size_t q) {
  for (size_t j = 0; j < 3; j++) {
    for (size_t i = 0; i < 3; i++) {
      M[q + j][p + i] = Mx[j][i];
    }
  }
}

void minor(__int128 M[6][6], __int128 m[5][5], size_t p, size_t q) {
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

__float128 det5(__int128 m[5][5]) {
  __float128 det = 1;
  __float128 total = 1;

  __float128 copy[5][5];
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
        __float128 t = copy[index][j];
        copy[index][j] = copy[i][j];
        copy[i][j] = t;
      }
      det = -det;
    }

    __float128 tmp[5];
    for (size_t j = 0; j < 5; j++) {
      tmp[j] = copy[i][j];
    }
    for (size_t j = i + 1; j < 5; j++) {
      __float128 a = tmp[i];
      __float128 b = copy[j][i];
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

void cofactor(__int128 M[6][6], __float128 C[6][6]) {
  for (size_t j = 0; j < 6; j++) {
    for (size_t i = 0; i < 6; i++) {
      __int128 m[5][5];
      minor(M, m, i, j);
      C[j][i] = det5(m) * powq(-1, i + j);
    }
  }
}

void transpose(__float128 M[6][6]) {
  for (size_t j = 0; j < 6; j++) {
    for (size_t i = j + 1; i < 6; i++) {
      __float128 tmp = M[j][i]; 
      M[j][i] = M[i][j];
      M[i][j] = tmp;
    }
  }
}

__float128 determinant(__int128 M[6][6]) {
  __float128 det = 1;
  __float128 total = 1;

  __float128 copy[6][6];
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
        __float128 t = copy[index][j];
        copy[index][j] = copy[i][j];
        copy[i][j] = t;
      }
      det = -det;
    }

    __float128 tmp[6];
    for (size_t j = 0; j < 6; j++) {
      tmp[j] = copy[i][j];
    }
    for (size_t j = i + 1; j < 6; j++) {
      __float128 a = tmp[i];
      __float128 b = copy[j][i];
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

void debugi(__int128 M[6][6]) {
  for (size_t j = 0; j < 6; j++) {
    for (size_t i = 0; i < 6; i++) {
      printf("%20lld", (ll) M[j][i]);
    }
    printf("\n");
  }
}

void debugq(__float128 M[6][6]) {
  for (size_t j = 0; j < 6; j++) {
    for (size_t i = 0; i < 6; i++) {
      printf("%20.5Lf", (ld) M[j][i]);
    }
    printf("\n");
  }
}

void inverse(__int128 M[6][6], __float128 I[6][6]) {
  __float128 d = determinant(M);
  printf("d = %lld\n", (ll) d);

  __float128 A[6][6];
  cofactor(M, A);
  printf("A:\n"); debugq(A);

  transpose(A);
  printf("A^T:\n"); debugq(A);

  for (size_t j = 0; j < 6; j++) {
    for (size_t i = 0; i < 6; i++) {
      I[j][i] = (__float128) A[j][i] / d;
    }
  }
}

void multiply(__float128 M[6][6], __int128 v[6], __float128 r[6]) {
  for (size_t j = 0; j < 6; j++) {
    __float128 rj = 0;
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
  __float128 x;
  __float128 y;
  bool futureA;
  bool futureB;
} Intersection;

// https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection
bool intersects(Hailstone *a, Hailstone *b, Intersection *p) {
  // L1 being defined by two distinct points (x1, y1) and (x2, y2)
  __int128 x1 = a->p.x;
  __int128 y1 = a->p.y;
  __int128 x2 = a->p.x + a->v.x;
  __int128 y2 = a->p.y + a->v.y;
  // L2 being defined by two distinct points (x3, y3) and (x4, y4)
  __int128 x3 = b->p.x;
  __int128 y3 = b->p.y;
  __int128 x4 = b->p.x + b->v.x;
  __int128 y4 = b->p.y + b->v.y;

  __int128 d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
  if (d == 0) {
    return false;
  }
 
  __int128 pxn = (x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4);
  __int128 pyn = (x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4);

  p->x = (__float128) pxn / d;
  p->y = (__float128) pyn / d;
  p->futureA = (p->x > x1) == (x2 > x1);
  p->futureB = (p->x > x3) == (x4 > x3);

  return true;
}

int part1(char *filename, __int128 min, __int128 max, bool debug) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  Array *hailstones = array_new(Hailstone);

  char buffer[BUFFER_LENGTH] = {0};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    StringArray *at_split = string_split(buffer, " @ "); 
    StringArray *positions = string_split(at_split->items[0], ", ");
    __int128 px = atoll(positions->items[0]);
    __int128 py = atoll(positions->items[1]);
    __int128 pz = atoll(positions->items[2]);
    StringArray *velocities = string_split(at_split->items[1], ", ");
    __int128 vx = atoll(velocities->items[0]);
    __int128 vy = atoll(velocities->items[1]);
    __int128 vz = atoll(velocities->items[2]);

    Hailstone hailstone = { { px, py, pz }, { vx, vy, vz } };
    array_push(hailstones, hailstone);

    string_array_free(velocities);
    string_array_free(positions);
    string_array_free(at_split);
  }

  fclose(fp);

  int sum = 0;
  for (size_t i = 0; i < hailstones->size - 1; i++) {
    Hailstone *a = array_get(hailstones, i);
    for (size_t j = i + 1; j < hailstones->size; j++) {
      Hailstone *b = array_get(hailstones, j);
      if (debug) printf("Hailstone A: %lld, %lld, %lld @ %lld, %lld, %lld\n", (ll) a->p.x, (ll) a->p.y, (ll) a->p.z, (ll) a->v.x, (ll) a->v.y, (ll) a->v.z);
      if (debug) printf("Hailstone B: %lld, %lld, %lld @ %lld, %lld, %lld\n", (ll) b->p.x, (ll) b->p.y, (ll) b->p.z, (ll) b->v.x, (ll) b->v.y, (ll) b->v.z);

      Intersection p;
      if (intersects(a, b, &p)) {
        if (p.futureA) {
          if (p.futureB) {
            if (min <= p.x && p.x <= max && min <= p.y && p.y <= max) {
              if (debug) printf("Hailstones' paths will cross inside the test area (at x=%.3Lf, y=%.3Lf).\n",  (ld) p.x, (ld) p.y);
              sum += 1;
            } else {
              if (debug) printf("Hailstones' paths will cross outside the test area (at x=%.3Lf, y=%.3Lf).\n", (ld) p.x, (ld) p.y);
            }
          } else {
            if (debug) printf("Hailstones' paths crossed in the past for hailstone B.\n");
          }
        } else if (p.futureB) {
          if (debug) printf("Hailstones' paths crossed in the past for hailstone A.\n");
        } else if (!p.futureA && !p.futureB) {
          if (debug) printf("Hailstones' paths crossed in the past for both hailstones.\n");
        }
      } else {
        if (debug) printf("Hailstones' paths are parallel; they never intersect.\n");
      }
      if (debug) printf("\n");
    }
  }

  printf("sum = %d\n", sum);

  array_free(hailstones);

  return sum;
}

__int128 part2(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  Array *hailstones = array_new(Hailstone);

  char buffer[BUFFER_LENGTH] = {0};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    StringArray *at_split = string_split(buffer, " @ "); 
    StringArray *positions = string_split(at_split->items[0], ", ");
    __int128 px = atoll(positions->items[0]);
    __int128 py = atoll(positions->items[1]);
    __int128 pz = atoll(positions->items[2]);
    StringArray *velocities = string_split(at_split->items[1], ", ");
    __int128 vx = atoll(velocities->items[0]);
    __int128 vy = atoll(velocities->items[1]);
    __int128 vz = atoll(velocities->items[2]);

    Hailstone hailstone = { { px, py, pz }, { vx, vy, vz } };
    array_push(hailstones, hailstone);

    string_array_free(velocities);
    string_array_free(positions);
    string_array_free(at_split);
  }

  fclose(fp);

  Hailstone *h0 = array_get(hailstones, 0);
  Hailstone *h1 = array_get(hailstones, 1);
  Hailstone *h2 = array_get(hailstones, 2);

  __int128 M0[3][3]; // ┌       ┐
  __int128 M1[3][3]; // | M0 M2 | 
  __int128 M2[3][3]; // | M1 M3 |
  __int128 M3[3][3]; // └       ┘

  cross_matrix(sub(h0->v, h1->v), M0);
  cross_matrix(sub(h0->v, h2->v), M1);
  cross_matrix(sub(h1->p, h0->p), M2);
  cross_matrix(sub(h2->p, h0->p), M3);

  __int128 M[6][6];
  merge(M, M0, 0, 0);
  merge(M, M1, 0, 3);
  merge(M, M2, 3, 0);
  merge(M, M3, 3, 3);
  printf("M:\n"); debugi(M);

  __float128 I[6][6];
  inverse(M, I);
  printf("I:\n"); debugq(I);

  __int128 v[6];
  Vec3 v0 = sub(cross(h1->p, h1->v), cross(h0->p, h0->v));
  Vec3 v1 = sub(cross(h2->p, h2->v), cross(h0->p, h0->v));
  v[0] = v0.x; v[1] = v0.y; v[2] = v0.z; v[3] = v1.x; v[4] = v1.y; v[5] = v1.z;
  printf("rhs: %lld %lld %lld %lld %lld %lld\n", (ll) v[0], (ll) v[1], (ll) v[2], (ll) v[3], (ll) v[4], (ll) v[5]);

  __float128 r[6];
  multiply(I, v, r);
  for (size_t i = 0; i < 6; i++) {
    r[i] = roundq(r[i]);
  }
  printf("r: %.Lf %.Lf %.Lf %.Lf %.Lf %.Lf\n", (ld) r[0], (ld) r[1], (ld) r[2], (ld) r[3], (ld) r[4], (ld) r[5]);

  __int128 sum = r[0] + r[1] + r[2];
  printf("sum = %lld\n", (ll) sum);

  array_free(hailstones);

  return (__int128) sum;
}

int main() {
  assert(part1("../../inputs/2023/day24/sample", 7, 27, true) == 2);
  assert(part1("../../inputs/2023/day24/data", 200000000000000, 400000000000000, false) == 18184);
  assert(part2("../../inputs/2023/day24/sample") == 47);
  assert(part2("../../inputs/2023/day24/data") == 557789988450159);

  return 0;
}
