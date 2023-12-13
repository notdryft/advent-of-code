#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "string.h"

#define BUFFER_LENGTH 1024

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
} Coord;

typedef struct {
  size_t size;
  Coord *coords;
} CoordsSet;

CoordsSet *coords_from(StringArray *schema, size_t rows, size_t cols, size_t x, size_t y) {
  size_t coords_size = 0;
  Coord *coords = (Coord *) malloc(sizeof(Coord) * 4);

  char *row = string_array_get(schema, y);
  if (can_go_north(row[x]) && y > 0) {
    char *previous_row = string_array_get(schema, y - 1);
    if (valid_going_north(previous_row[x])) {
      Coord coord = { .x = x, .y = y - 1 }; 
      coords[coords_size++] = coord;
    }
  }
  if (can_go_east(row[x]) && x + 1 < cols) {
    if (valid_going_east(row[x + 1])) {
      Coord coord = { .x = x + 1, .y = y }; 
      coords[coords_size++] = coord;
    }
  }
  if (can_go_south(row[x]) && y + 1 < rows) {
    char *next_row = string_array_get(schema, y + 1);
    if (valid_going_south(next_row[x])) {
      Coord coord = { .x = x, .y = y + 1 }; 
      coords[coords_size++] = coord;
    }
  }
  if (can_go_west(row[x]) && x > 0) {
    if (valid_going_west(row[x - 1])) {
      Coord coord = { .x = x - 1, .y = y }; 
      coords[coords_size++] = coord;
    }
  }

  CoordsSet *set = (CoordsSet *) malloc(sizeof(CoordsSet));
  set->size = coords_size;
  set->coords = coords;

  return set;
}

void free_coords_set(CoordsSet *set) {
  if (set != NULL) {
    if (set->coords != NULL) {
      free(set->coords);
    }
    free(set);
  }
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
  CoordsSet *set = coords_from(schema, rows, cols, x, y);
  //printf("size: %zu\n", set->size);
  for (size_t i = 0; i < set->size; i++) {
    Coord c = set->coords[i];
    //char *yrow = string_array_get(schema, y);
    //char *cyrow = string_array_get(schema, c.y);
    //printf("from { %zu, %zu } = '%c' to { %zu, %zu } = '%c' \n", x, y, yrow[x], c.x, c.y, cyrow[c.x]);
    int step = steps[c.y][c.x];
    if (step == 0 && count > 2) {
      //printf("highest = %d\n", count);
      *highest = count;
    } else if (step == -1) {
      steps[c.y][c.x] = count;
      traverse_rec(schema, steps, rows, cols, c.x, c.y, count + 1, highest);
    }
  }
  free_coords_set(set);
}

CoordsSet *traverse_steps(StringArray *schema, int **steps, size_t rows, size_t cols, int highest) {
  size_t coords_size = 0;
  Coord *coords = (Coord *) malloc(sizeof(Coord) * 10000);

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
  //printf("(%zu, %zu)\n", x, y);
  Coord coord = { .x = x, .y = y };
  coords[coords_size++] = coord;

  stop = false;
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
      Coord coord = { .x = x, .y = y };
      coords[coords_size++] = coord;
    }
  }

  CoordsSet *set = (CoordsSet *) malloc(sizeof(CoordsSet));
  set->size = coords_size;
  set->coords = coords;

  return set;
}

int is_left(Coord *edge1, Coord *edge2, size_t x, size_t y) {
  return (edge2->x - edge1->x) * (y - edge1->y) -
         (x - edge1->x) * (edge2->y - edge1->y);
}

// https://en.wikipedia.org/wiki/Point_in_polygon#Winding_number_algorithm
// https://stackoverflow.com/a/37703647
// and https://forums.codeguru.com/showthread.php?497679-To-check-if-a-point-is-inside-a-polygon
int winding_number(CoordsSet *set, size_t x, size_t y) {
  int wn = 0;
  for (size_t i = 0; i < set->size; i++) {
    Coord c = set->coords[i];
    Coord n = (i == set->size - 1) ? set->coords[0] : set->coords[i + 1];
    if (c.y <= y) {
      if (n.y > y) {
        int l = is_left(&c, &n, x, y); 
        if (l > 0) {
          wn++;
        } else if (l == 0) {
          return 0;
        }
      }
    } else {
      if (n.y <= y) {
        int l = is_left(&c, &n, x, y);
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
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  StringArray *schema = string_array_new();

  char buffer[BUFFER_LENGTH] = {0};
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

  int **steps = (int **) malloc(sizeof(int *) * rows);
  for (size_t j = 0; j < rows; j++) {
    steps[j] = (int *) malloc(sizeof(int) * cols);
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
  if (fp == NULL) {
    fprintf(stderr, "Error: could not open file %s\n", filename);
    return 1;
  }

  StringArray *schema = string_array_new();

  char buffer[BUFFER_LENGTH] = {0};
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

  int **steps = (int **) malloc(sizeof(int *) * rows);
  for (size_t j = 0; j < rows; j++) {
    steps[j] = (int *) malloc(sizeof(int) * cols);
    memset(steps[j], -1, sizeof(int) * cols);
  }

  printf("1\n");
  steps[sy][sx] = 0;
  int highest = -1;
  traverse_rec(schema, steps, rows, cols, sx, sy, 1, &highest);
  debug_steps(steps, rows, cols);

  CoordsSet *edges = traverse_steps(schema, steps, rows, cols, highest);
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
    }
    printf("%s\n", row);
  }
  printf("inside = %d\n", inside);

  free_coords_set(edges);

  string_array_free(schema);
  for (size_t j = 0; j < rows; j++) {
    free(steps[j]);
  }
  free(steps);

  return inside;
}

int main() {
  assert(part1("../../inputs/2023/day10/part1_test") == 8);
  assert(part1("../../inputs/2023/day10/data") == 6768);
  assert(part2("../../inputs/2023/day10/part2_1_test") == 4);
  assert(part2("../../inputs/2023/day10/part2_2_test") == 8);
  assert(part2("../../inputs/2023/day10/part2_3_test") == 10);
  assert(part2("../../inputs/2023/day10/data") == 351);

  return 0;
}
