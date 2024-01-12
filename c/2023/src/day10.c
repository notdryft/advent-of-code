#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "string.h"

constexpr size_t BUFFER_LENGTH = 1024;

bool can_go_north(char c) {
  return c == 'S' || c == '|' || c == 'L' || c == 'J';
}

bool can_go_east(char c) {
  return c == 'S' || c == '-' || c == 'L' || c == 'F';
}

bool can_go_south(char c) {
  return c == 'S' || c == '|' || c == '7' || c == 'F';
}

bool can_go_west(char c) {
  return c == 'S' || c == '-' || c == 'J' || c == '7';
}

bool valid_going_north(char c) {
  return c == 'S' || c == '|' || c == '7' || c == 'F';
}

bool valid_going_east(char c) {
  return c == 'S' || c == '-' || c == 'J' || c == '7';
}

bool valid_going_south(char c) {
  return c == 'S' || c == '|' || c == 'L' || c == 'J';
}

bool valid_going_west(char c) {
  return c == 'S' || c == '-' || c == 'L' || c == 'F';
}

bool is_edge(char c) {
  return c == 'S' || c == 'L' || c == 'F' || c == 'J' || c == '7';
}

typedef struct {
  size_t x;
  size_t y;
} Vec2;

Array *vecs_from(StringArray *schema, size_t rows, size_t cols, size_t x, size_t y) {
  Array *vecs = array_new(Vec2);

  char *row = string_array_get(schema, y);
  if (can_go_north(row[x]) && y > 0) {
    char *previous_row = string_array_get(schema, y - 1);
    if (valid_going_north(previous_row[x])) {
      array_push(vecs, &(Vec2) { x, y - 1 });
    }
  }
  if (can_go_east(row[x]) && x + 1 < cols) {
    if (valid_going_east(row[x + 1])) {
      array_push(vecs, &(Vec2) { x + 1, y });
    }
  }
  if (can_go_south(row[x]) && y + 1 < rows) {
    char *next_row = string_array_get(schema, y + 1);
    if (valid_going_south(next_row[x])) {
      array_push(vecs, &(Vec2) { x, y + 1 });
    }
  }
  if (can_go_west(row[x]) && x > 0) {
    if (valid_going_west(row[x - 1])) {
      array_push(vecs, &(Vec2) { x - 1, y });
    }
  }

  return vecs;
}

void debug_steps(int **steps, size_t rows, size_t cols) {
  for (size_t i = 0; i < cols * 4; i++) {
    printf(">");
  }
  printf("\n");
  for (size_t j = 0; j < rows; j++) {
    for (size_t i = 0; i < cols; i++) {
      if (steps[j][i] == -1) {
        printf("    ");
      } else {
        printf("%4d", steps[j][i]);
      }
    }
    printf("\n");
  }
  for (size_t i = 0; i < cols * 4; i++) {
    printf("<");
  }
  printf("\n");
}

void traverse_rec(StringArray *schema, int **steps, size_t rows, size_t cols, size_t x, size_t y, int count, int *highest) {
  //printf("%d\n", count);
  Array *vecs = vecs_from(schema, rows, cols, x, y);
  //printf("size: %zu\n", vecs->size);
  for (size_t i = 0; i < vecs->size; i++) {
    Vec2 *u = array_get(vecs, i);
    //char *yrow = string_array_get(schema, y);
    //char *vyrow = string_array_get(schema, v->y);
    //printf("from { %zu, %zu } = '%c' to { %zu, %zu } = '%c' \n", x, y, yrow[x], v->x, v->y, vyrow[v->x]);
    int step = steps[u->y][u->x];
    if (step == 0 && count > 2) {
      //printf("highest = %d\n", count);
      *highest = count;
    } else if (step == -1) {
      steps[u->y][u->x] = count;
      traverse_rec(schema, steps, rows, cols, u->x, u->y, count + 1, highest);
    }
  }
  array_free(vecs);
}

Array *traverse_steps(StringArray *schema, int **steps, size_t rows, size_t cols, int highest) {
  Array *vecs = array_new(Vec2);

  bool stop = false;
  size_t x = 0, y = 0;
  for (size_t j = 0; j < rows && !stop; j++) {
    for (size_t i = 0; i < cols && !stop; i++) {
      if (steps[j][i] == 0) {
        x = i;
        y = j;
        stop = true;
      }
    }
  }

  printf("Start at (%zu, %zu)\n", x, y);
  array_push(vecs, &(Vec2) { x, y });

  int count = 0;
  while (1) {
    int next = (count + 1) % highest;
    if (y > 0 && steps[y - 1][x] == next) {
      y--;
    } else if (x + 1< cols && steps[y][x + 1] == next) {
      x++;
    } else if (y + 1 < rows && steps[y + 1][x] == next) {
      y++;
    } else if (x > 0 && steps[y][x - 1] == next) {
      x--;
    } else {
      printf("unknown case at (%zu, %zu)\n", x, y);
      exit(42);
    }
    if (steps[y][x] == 0) {
      break;
    }
    count = next;
    char *row = string_array_get(schema, y);
    if (is_edge(row[x])) {
      //printf("(%zu, %zu)\n", x, y);
      array_push(vecs, &(Vec2) { x, y });
    }
  }

  return vecs;
}

int is_left(Vec2 *edge1, Vec2 *edge2, size_t x, size_t y) {
  return (edge2->x - edge1->x) * (y - edge1->y) -
         (x - edge1->x) * (edge2->y - edge1->y);
}

// https://en.wikipedia.org/wiki/Point_in_polygon#Winding_number_algorithm
// https://stackoverflow.com/a/37703647
// and https://forums.codeguru.com/showthread.php?497679-To-check-if-a-point-is-inside-a-polygon
int winding_number(Array *vecs, size_t x, size_t y) {
  int wn = 0;
  for (size_t i = 0; i < vecs->size; i++) {
    Vec2 *c = array_get(vecs, i);
    Vec2 *n = (i == vecs->size - 1) ? array_get(vecs, 0) : array_get(vecs, i + 1);
    if (c->y <= y) {
      if (n->y > y) {
        int l = is_left(c, n, x, y);
        if (l > 0) {
          wn++;
        } else if (l == 0) {
          return 0;
        }
      }
    } else {
      if (n->y <= y) {
        int l = is_left(c, n, x, y);
        if (l < 0) {
          wn--;
        } else if (l == 0) {
          return 0;
        }
      }
    }
  }
  return wn;
}

int part1(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == nullptr) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  StringArray *schema = string_array_new();

  char buffer[BUFFER_LENGTH] = {};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    string_array_push(schema, buffer);
  }

  fclose(fp);

  size_t rows = strlen(string_array_get(schema, 0));
  size_t cols = schema->size;

  for (size_t j = 0; j < rows; j++) {
    char *row = string_array_get(schema, j);
    printf("%s\n", row);
  }

  printf("schema = %zux%zu\n", rows, cols);

  size_t sx = 0, sy = 0;
  bool stop = false;
  for (size_t j = 0; j < rows && !stop; j++) {
    char *row = string_array_get(schema, j);
    for (size_t i = 0; i < cols && !stop; i++) {
      if (row[i] == 'S') {
        sx = i;
        sy = j;
        stop = true;
      }
    }
  }
  printf("Start at (%zu, %zu)\n", sx, sy);

  int **steps = malloc(sizeof(int *) * rows);
  for (size_t j = 0; j < rows; j++) {
    steps[j] = malloc(sizeof(int) * cols);
    memset(steps[j], -1, sizeof(int) * cols);
  }

  steps[sy][sx] = 0;
  int highest = -1;
  traverse_rec(schema, steps, rows, cols, sx, sy, 1, &highest);
  //debug_steps(steps, rows, cols);

  printf("loop = %d\n", highest);
  int result = (highest % 2 == 0) ? highest / 2 : (highest + 1) / 2;
  printf("result = %d\n", result);

  string_array_free(schema);
  for (size_t j = 0; j < cols; j++) {
    free(steps[j]);
  }
  free(steps);

  return result;
}

int part2(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == nullptr) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  StringArray *schema = string_array_new();

  char buffer[BUFFER_LENGTH] = {};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    string_array_push(schema, buffer);
  }

  fclose(fp);

  size_t cols = strlen(string_array_get(schema, 0));
  size_t rows = schema->size;

  for (size_t j = 0; j < rows; j++) {
    char *row = string_array_get(schema, j);
    printf("%s\n", row);
  }

  printf("schema = %zux%zu\n", rows, cols);

  size_t sx = 0, sy = 0;
  bool stop = false;
  for (size_t j = 0; j < rows && !stop; j++) {
    char *row = string_array_get(schema, j);
    for (size_t i = 0; i < cols && !stop; i++) {
      if (row[i] == 'S') {
        sx = i;
        sy = j;
        stop = true;
      }
    }
  }
  printf("Start at (%zu, %zu)\n", sx, sy);

  int **steps = malloc(sizeof(int *) * rows);
  for (size_t j = 0; j < rows; j++) {
    steps[j] = malloc(sizeof(int) * cols);
    for (size_t i = 0; i < cols; i++) {
      steps[j][i]  = -1;
    }
  }

  printf("1\n");
  steps[sy][sx] = 0;
  int highest = -1;
  traverse_rec(schema, steps, rows, cols, sx, sy, 1, &highest);
  debug_steps(steps, rows, cols);

  Array *edges = traverse_steps(schema, steps, rows, cols, highest);
  printf("edges = %zu\n", edges->size);

  int inside = 0;
  for (size_t j = 0; j < rows; j++) {
    char *row = string_array_get(schema, j);
    for (size_t i = 0; i < cols; i++) {
      //printf("(%zu, %zu)\n", i, j);
      if (steps[j][i] == -1) { // ignoring pipes that are not part of the loop
        if (winding_number(edges, i, j) != 0) {
          row[i] = 'I';
          inside++;
        } else {
          row[i] = 'O';
        }
      }
      if (row[i] == 'O') {
        printf("\033[97;1m%c\033[0m", row[i]);
      } else if (row[i] == 'I') {
        printf("\033[91;1m%c\033[0m", row[i]);
      } else {
        printf("%c", row[i]);
      }
    }
    printf("\n");
  }
  printf("inside = %d\n", inside);

  array_free(edges);
  for (size_t j = 0; j < rows; j++) {
    free(steps[j]);
  }
  free(steps);
  string_array_free(schema);

  return inside;
}

int main(void) {
  assert(part1("../../inputs/2023/day10/sample1") == 8);
  assert(part1("../../inputs/2023/day10/data") == 6768);
  assert(part2("../../inputs/2023/day10/sample2") == 4);
  assert(part2("../../inputs/2023/day10/sample3") == 8);
  assert(part2("../../inputs/2023/day10/sample4") == 10);
  assert(part2("../../inputs/2023/day10/data") == 351);

  return 0;
}
