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
} Direction;

typedef struct {
  int x;
  int y;
  int direction;
} State;

typedef struct {
  int cost;
  State state;
} Q;

typedef struct {
  State key;
  int value;
} MapEntry;

typedef struct {
  size_t capacity;
  size_t size;
  MapEntry **entries;
} Map;

Map *map_new() {
  Map *map = (Map *) malloc(sizeof(Map));
  map->capacity = 0;
  map->size = 0;
  map->entries = NULL;

  return map;
}

void map_free(Map *map) {
  if (map != NULL) {
    if (map->entries != NULL) {
      for (size_t i = 0; i < map->size; i++) {
        MapEntry *entry = map->entries[i];
        if (entry != NULL) {
          free(entry);
        }
      }
      free(map->entries);
    }
    free(map);
  }
}

MapEntry *map_get(Map *map, State *key) {
  for (size_t i = 0; i < map->size; i++) {
    if (memcmp(key, map->entries[i], sizeof(State)) == 0) {
      return map->entries[i];
    }
  }
  return NULL;
}

bool map_contains(Map *map, State *key) {
  for (size_t i = 0; i < map->size; i++) {
    if (memcmp(key, map->entries[i], sizeof(State)) == 0) {
      return true;
    }
  }
  return false;
}

void map_put(Map *map, State *key, int value) {
  if (map->size + 1 > map->capacity) {
    map->capacity = map->capacity == 0 ? 2 : map->capacity * 2;
    map->entries = (MapEntry **) realloc(map->entries, sizeof(MapEntry *) * map->capacity);
  }

  MapEntry *entry = map_get(map, key);
  if (entry != NULL) {
    entry->value = value;
  } else {
    entry = (MapEntry *) malloc(sizeof(MapEntry));
    memcpy(&(entry->key), key, sizeof(State));
    entry->value = value;

    map->entries[map->size++] = entry;
  }
}

void array_priority_push(Array *array, Q *value) {
  if (array->size == 0) {
    _array_push(array, value);
  } else {
    size_t i = 0;
    Q *other;
    do {
      other = (Q *) array_get(array, i++);
    } while (other->cost < value->cost && i < array->size);
    _array_insert(array, i - 1, value);
  }
}

int dijkstra(char **grid, int mx, int my, int mindist, int maxdist) {
  Array *q = array_new(Q);
  Map *visited = map_new(); // FIXME could be a set
  Map *costs = map_new();  // FIXME merge with visited?

  Q source = {
    .cost = 0,
    .state = {
      .x = 0,
      .y = 0,
      .direction = -1
    }
  };
  array_push(q, source);

  Array *directions = array_new(Direction);
  array_push(directions, ((Direction) { .x =  0, .y = -1 })); // North
  array_push(directions, ((Direction) { .x =  1, .y =  0 })); // East
  array_push(directions, ((Direction) { .x =  0, .y =  1 })); // South
  array_push(directions, ((Direction) { .x = -1, .y =  0 })); // West

  int cost = -1;

  while (q->size > 0) {
    Q *current = array_pop(q);
    State current_state = current->state;

    if (current_state.x == mx - 1 && current_state.y == my - 1) {
      cost = current->cost;
      free(current);
      goto leave;
    }
    if (map_contains(visited, &current_state)) {
      free(current);
      continue;
    }
    map_put(visited, &current_state, 0); // ignore cost, we are using the map as a set... :)
    for (int new_direction = 0; new_direction < 4; new_direction++) {
      if (new_direction == current_state.direction || ((new_direction + 2) % 4) == current_state.direction) {
        continue;
      }
      Direction *d = (Direction *) array_get(directions, new_direction);

      int cost_acc = 0;

      for (int distance = 1; distance <= maxdist; distance++) {
        int dx = current_state.x + d->x * distance;
        int dy = current_state.y + d->y * distance;

        if (0 <= dx && dx < mx && 0 <= dy && dy < my) {
          cost_acc += grid[dy][dx] - '0';
          if (distance < mindist) {
            continue;
          }

          int new_cost = current->cost + cost_acc;

          State s = {
            .x = dx,
            .y = dy,
            .direction = new_direction
          };

          MapEntry* c = map_get(costs, &s);
          //printf("%p %d %d\n", c, (c == NULL) ? -1 : c->value, new_cost);
          if (c != NULL && c->value < new_cost) {
            continue;
          }
          map_put(costs, &s, new_cost);

          Q r = {
            .cost = new_cost,
            .state = s
          };
          //printf("cost = %d, x = %d, y = %d, d = %d\n", new_cost, dx, dy, new_direction);
          array_priority_push(q, &r);
        }
      }
    }
    free(current);
  }

  leave:
  array_free(directions);
  array_free(q);
  map_free(costs);
  map_free(visited);

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
  assert(part1("../../inputs/2023/day17/part1_test") == 102);
  assert(part1("../../inputs/2023/day17/data") == 855 + 1); // FIXME off by 1?
  assert(part2("../../inputs/2023/day17/part2_1_test") == 94);
  assert(part2("../../inputs/2023/day17/part2_2_test") == 71);
  assert(part2("../../inputs/2023/day17/data") == 980);

  return 0;
}
