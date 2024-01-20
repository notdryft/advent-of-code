#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "string.h"

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
    array_push(array, value);
  } else {
    size_t i = 0;
    while (i < array->size) {
      Q *other = array_get(array, i);
      if (other->cost > value->cost) {
        break;
      }
      i++;
    }
    array_insert(array, i, value);
  }
}

#define dim(x, y, d) (x + y*my + (d+1)*my*mx)

int dijkstra(char **grid, int mx, int my, int mindist, int maxdist) {
  Array *q = array_new(Q);

  int *costs = malloc(sizeof(*costs) * 5 * my * mx);
  bool *visited = malloc(sizeof(*visited) * 5 * my * mx);
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
  array_push(q, &source);

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
      goto defer;
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

          debug("costs(%d, %d, %d) = %d\n", dx, dy, direction, costs[dim(dx, dy, direction)]);
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
          debug("cost = %d, x = %d, y = %d, d = %d\n", new_cost, dx, dy, direction);
          array_priority_push(q, &r);
        }
      }
    }
    free(current);
  }

  defer:
  free(costs);
  free(visited);
  array_free(q);

  return cost;
}

int part1(StringArray *lines) {
  int mx = strlen(lines->items[0]);
  int my = lines->size;

  return dijkstra(lines->items, mx, my, 1, 3);
}

int part2(StringArray *lines) {
  int mx = strlen(lines->items[0]);
  int my = lines->size;

  return dijkstra(lines->items, mx, my, 4, 10);
}

int main(void) {
  test_case(day17, part1, sample1, 102);
  test_case(day17, part1, data, 855);
  test_case(day17, part2, sample1, 94);
  test_case(day17, part2, sample2, 71);
  test_case(day17, part2, data, 980);

  return 0;
}
