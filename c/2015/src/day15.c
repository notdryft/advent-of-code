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
  i64 capacity;
  i64 durability;
  i64 flavor;
  i64 texture;
  i64 calories;
} Ingredient;

void print_ingredients(Array *ingredients) {
  printf("Array{\n");
  array_foreach(Ingredient *ingredient, ingredients) {
    printf(
      "  Ingredient{ capacity = %lld, durability = %lld, flavor = %lld, texture = %lld, calories = %lld }\n",
      ingredient->capacity, ingredient->durability, ingredient->flavor, ingredient->texture, ingredient->calories
    );
  }
  printf("}\n");
}

void rec(Array *ingredients, Array *quantities, size_t index, i64 quantity, Array *values, i64 target) {
  if (quantity == 0 && ingredients->size == quantities->size) {
    debugf(i64_array_print, quantities);

    i64 capacity = 0, durability = 0, flavor = 0, texture = 0, calories = 0;
    for (size_t i = 0; i < ingredients->size; i++) {
      Ingredient *ingredient = array_get(ingredients, i);
      i64 qty = i64_array_get(quantities, i);

      capacity += qty * ingredient->capacity;
      durability += qty * ingredient->durability;
      flavor += qty * ingredient->flavor;
      texture += qty * ingredient->texture;
      calories += qty * ingredient->calories;
    }
    capacity = capacity > 0 ? capacity : 0;
    durability = durability > 0 ? durability : 0;
    flavor = flavor > 0 ? flavor : 0;
    texture = texture > 0 ? texture : 0;

    i64 value = capacity * durability * flavor * texture;
    if (value != 0 && (target == -1 || target == calories)) {
      array_push_rval(values, value);
    }
  } else if (index < ingredients->size) {
    for (i64 i = 1; i <= quantity; i++) {
      array_push_rval(quantities, i);
      rec(ingredients, quantities, index + 1, quantity - i, values, target);
      array_remove_last(quantities);
    }
  }
}

i64 compute(StringArray *lines, i64 target) {
  Array *ingredients = array_new(Ingredient);
  string_array_foreach (char *line, lines) {
    debug("%s\n", line);

    i64 capacity, durability, flavor, texture, calories;
    if (
      sscanf(
        line,
        "%*[^:]: capacity %lld, durability %lld, flavor %lld, texture %lld, calories %lld",
        &capacity, &durability, &flavor, &texture, &calories
      ) == 5
    ) {
      array_push(ingredients, &(Ingredient) {
        .capacity = capacity,
        .durability = durability,
        .flavor = flavor,
        .texture = texture,
        .calories = calories
      });
    }
  }
  debugf(print_ingredients, ingredients);

  Array *quantities = array_new(i64);
  Array *values = array_new(i64);
  rec(ingredients, quantities, 0, 100, values, target);
  debugf(i64_array_print, values);

  i64 result = -1;
  array_foreach (i64 *value, values) {
    if (*value > result) {
      result = *value;
    }
  }
  array_free(ingredients);
  array_free(values);

  return result;
}

i64 part1(StringArray *lines) {
  return compute(lines, -1);
}

i64 part2(StringArray *lines, i64 target) {
  return compute(lines, target);
}

int main(void) {
  test_case(day15, part1, sample, 62842880);
  test_case(day15, part1, data, 222870);
  test_case(day15, part2, sample, 57600000, 500);
  test_case(day15, part2, data, 117936, 500);

  return 0;
}
