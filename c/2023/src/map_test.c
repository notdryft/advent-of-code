#include <stdio.h>

#include "map.h"

void map_tests() {
  Map *map = map_new();
  map_print(map);
  string_map_print(map);

  printf("contains key \"%s\" = %s\n", "very long key 1", map_contains_key(map, "very long key 1") ? "true" : "false");
  map_put(map, "very long key 1", "very long value seven");
  map_print(map);
  string_map_print(map);
  printf(
    "contains key \"%s\" = %s, value = \"%s\"\n",
    "key1",
    map_contains_key(map, "key1") ? "true" : "false",
    (char *) map_get(map, "key1")
  );

  printf("contains key \"%s\" = %s\n", "very long key 2", map_contains_key(map, "very long key 2") ? "true" : "false");
  map_put(map, "very long key 2", "very long value three");
  map_print(map);
  string_map_print(map);
  printf(
    "contains key \"%s\" = %s, value = \"%s\"\n",
    "very long key 2",
    map_contains_key(map, "very long key 2") ? "true" : "false",
    (char *) map_get(map, "very long key 2")
  );

  map_free(map);
}

int main() {
  printf("// Map\n");
  map_tests();

  return 0;
}
