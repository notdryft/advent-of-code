#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "string.h"

#define BUFFER_LENGTH 1024

typedef struct {
  int x;
  int y;
  int direction;
} State;

typedef struct {
  int cost;
  State state;
} Q;

void array_priority_push(Array *array, Q *value) {
  if (array->size == 0) {
    _array_push(array, value);
  } else {
    size_t i = 0;
    Q *other;
    do {
      other = (Q *) array_get(array, i++);
    } while (other->cost < value->cost && i <= array->size);
    _array_insert(array, i - 1, value);
  }
}

#define dim(x, y, d) (x + y*my + (d+1)*my*mx)

int dijkstra(char **grid, int mx, int my, int mindist, int maxdist) {
  Array *q = array_new(Q);

  int *costs = (int *) malloc(sizeof(int) * 5 * my * mx);
  bool *visited = (bool *) malloc(sizeof(bool) * 5 * my * mx);
  for (int d = 0; d < 5; d++) {
    for (int y = 0; y < my; y++) {
      for (int x = 0; x < mx; x++) {
        costs[dim(x, y, d-1)] = INT_MAX;
        visited[dim(x, y, d-1)] = false;
      }
    }
  }

  Q source = {
    .cost = 0,
    .state = {
      .x = 0,
      .y = 0,
      .direction = -1
    }
  };
  array_push(q, source);

  int directions[4][2] = {
    // x,  y
    {  0, -1 }, // North
    {  1,  0 }, // East
    {  0,  1 }, // South
    { -1,  0 }  // West
  };

  int cost = -1;

  while (q->size > 0) {
    Q *current = array_pop(q);
    int x = current->state.x;
    int y = current->state.y;
    int dd = current->state.direction;

    if (x == mx - 1 && y == my - 1) {
      cost = current->cost;
      free(current);
      goto leave;
    }

    //printf("visited(%d, %d, %d) = %s\n", x, y, dd, visited[dim(x, y, dd)] ? "true" : "false");
    if (visited[dim(x, y, dd)]) {
      free(current);
      continue;
    }
    visited[dim(x, y, dd)] = true;

    for (int direction = 0; direction < 4; direction++) {
      if (direction == dd || ((direction + 2) % 4) == dd) {
        continue;
      }

      int cost_acc = 0;

      for (int distance = 1; distance <= maxdist; distance++) {
        int dx = x + directions[direction][0] * distance;
        int dy = y + directions[direction][1] * distance;

        if (0 <= dx && dx < mx && 0 <= dy && dy < my) {
          cost_acc += grid[dy][dx] - '0';
          if (distance < mindist) {
            continue;
          }

          int new_cost = current->cost + cost_acc;

          //printf("costs(%d, %d, %d) = %d\n", dx, dy, d, costs[dim(dx, dy, d)]);
          if (costs[dim(dx, dy, direction)] <= new_cost) {
            continue;
          }
          costs[dim(dx, dy, direction)] = new_cost;

          Q r = {
            .cost = new_cost,
            .state = {
              .x = dx,
              .y = dy,
              .direction = direction
            }
          };
          //printf("cost = %d, x = %d, y = %d, d = %d\n", new_cost, dx, dy, new_direction);
          array_priority_push(q, &r);
        }
      }
    }
    free(current);
  }

  leave:
  free(costs);
  free(visited);
  array_free(q);

  return cost;
}

int part1(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  StringArray *lines = string_array_new();

  char buffer[BUFFER_LENGTH] = {0};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    string_array_push(lines, buffer);
  }

  fclose(fp);

  int mx = strlen(lines->items[0]);
  int my = lines->size;

  int cost = dijkstra(lines->items, mx, my, 1, 3);
  printf("cost = %d\n", cost);

  string_array_free(lines);

  return cost;
}

int part2(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  StringArray *lines = string_array_new();

  char buffer[BUFFER_LENGTH] = {0};
  while (fgets(buffer, BUFFER_LENGTH, fp)) {
    size_t buffer_len = strlen(buffer);
    buffer[buffer_len - 1] = '\0';

    string_array_push(lines, buffer);
  }

  fclose(fp);

  int mx = strlen(lines->items[0]);
  int my = lines->size;

  int cost = dijkstra(lines->items, mx, my, 4, 10);
  printf("cost = %d\n", cost);

  string_array_free(lines);

  return cost;
}

int main() {
  assert(part1("../../inputs/2023/day17/sample1") == 102);
  assert(part1("../../inputs/2023/day17/data") == 855);
  assert(part2("../../inputs/2023/day17/sample1") == 94);
  assert(part2("../../inputs/2023/day17/sample2") == 71);
  assert(part2("../../inputs/2023/day17/data") == 980);

  return 0;
}
