#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "string.h"

#define hash(key) ((size_t)(strlen(key)>1?(26+(key[0]-'a')+(key[1]-'a')*26):(key[0]-'a')))

enum ModuleType {
  OUTPUT,
  BROADCASTER,
  FLIP_FLOP,
  CONJUNCTION
};

typedef struct {
  char *name;
  enum ModuleType module_type;
  bool on;
  StringArray *connections;
  int *memory;
  Array *inputs;
} Module;

enum SignalType {
  LOW,
  HIGH
};

typedef struct {
  Module *source;
  enum SignalType signal_type;
  Module *destination;
} Signal;

void signal_print(Signal *signal) {
  printf("%s -%s-> %s\n", signal->source == nullptr ? "button" : signal->source->name, signal->signal_type == LOW ? "low" : "high", signal->destination->name);
}

void module_print(Module *module) {
  char *module_type = module->module_type == 0 ? " ?" : (module->module_type == 1 ? " " : (module->module_type == 2 ? " %" : " &"));
  printf("{%s%s ", module_type, module->name);
  if (module->connections->size > 0) {
    printf("-> ");
  } else {
    printf("X");
  }
  for (size_t i = 0; i < module->connections->size; i++) {
    printf("%s", module->connections->items[i]);
    if (i < module->connections->size - 1) printf(", ");
  }
  if (module->module_type == CONJUNCTION) {
    printf(" [ ");
    for (size_t i = 0; i < module->inputs->size; i++) {
      size_t input = *(size_t *) array_get(module->inputs, i);
      char name[3] = { 0 };
      if (input < 26) {
        name[0] = 'a' + input;
      } else {
        name[0] = 'a' + (input - 26) % 26;
        name[1] = 'a' + (input - 26) / 26;
      }
      printf("%s: %s", name, module->memory[input] == LOW ? "LOW" : "HIGH");
      if (i < module->inputs->size - 1) printf(", ");
    }
    printf(" ]");
  }
  printf(" }");
}

void array_module_print(Array *modules) {
  printf("Modules[ ");
  for (size_t i = 0; i < modules->size; i++) {
    Module *module = array_get(modules, i);
    module_print(module);
    if (i < modules->size - 1) printf(", ");
  }
  printf(" ]\n");
}

Module *find_module(Array *modules, char *name) {
  for (size_t i = 0; i < modules->size; i++) {
    Module *module = array_get(modules, i);
    if (strcmp(module->name, name) == 0) {
      return module;
    }
  }
  return nullptr;
}

Module init_module(char *name) {
  Module module = {
    .name = strdup(name),
    .connections = string_array_new(),
    .memory = calloc(26 * 27, sizeof(*module.memory)),
    .inputs = array_new(size_t)
  };

  return module;
}

unsigned long long part1(StringArray *lines) {
  Array *modules = array_new(Module);
  string_array_foreach (char *line, lines) {
    StringArray *split = string_split(line, " -> ");
    char *name = split->items[0];
    if (name[0] == '%' || name[0] == '&') {
      name = name + 1;
    }

    Module module = init_module(name);
    if (line[0] == '%') {
      module.module_type = FLIP_FLOP;
    } else if (line[0] == '&') {
      module.module_type = CONJUNCTION;
    } else {
      module.module_type = BROADCASTER;
    }
    array_push(modules, &module);

    string_array_free(split);
  }
  debugf(array_module_print, modules);

  string_array_foreach (char *line, lines) {
    StringArray *split = string_split(line, " -> ");
    char *name = split->items[0];
    if (name[0] == '%' || name[0] == '&') {
      name = name + 1;
    }

    StringArray *csplit = string_split(split->items[1], ", ");

    Module *module = find_module(modules, name);
    string_array_foreach(char *connection, csplit) {
      Module *c = find_module(modules, connection);
      if (c == nullptr) {
        Module output = init_module(connection);
        array_push(modules, &output);
      }
      string_array_push(module->connections, connection);
    }

    string_array_free(csplit);
    string_array_free(split);
  }
  debugf(array_module_print, modules);

  for (size_t i = 0; i < modules->size; i++) {
    Module *m = array_get(modules, i);
    for (size_t j = 0; j < m->connections->size; j++) {
      Module *c = find_module(modules, m->connections->items[j]);
      if (c->module_type == CONJUNCTION) {
        array_push_rval(c->inputs, hash(m->name));
      }
    }
  }
  debugf(array_module_print, modules);

  unsigned long long lows = 0;
  unsigned long long highs = 0;
  unsigned long long times = 1000;

  for (size_t i = 0; i < times; i++) {
    Array *q = array_new(Signal);
    array_push(q, &(Signal) { .source = nullptr, .signal_type = LOW, .destination = find_module(modules, "broadcaster") });
    while (q->size > 0) {
      Signal *s = array_shift(q);
      if (s->destination->module_type == OUTPUT) {
        goto defer;
      } else if (s->destination->module_type == BROADCASTER) {
        tracef(signal_print, s);
        lows++;
        for (size_t j = 0; j < s->destination->connections->size; j++) {
          lows++;
          Module *m = find_module(modules, s->destination->connections->items[j]);
          Signal ns = { .source = s->destination, .signal_type = LOW, .destination = m };
          tracef(signal_print, &ns);
          array_push(q, &ns);
        }
      } else if (s->destination->module_type == FLIP_FLOP) {
        if (s->signal_type == HIGH) {
          goto defer;
        }
        s->destination->on = !s->destination->on;
        enum SignalType st;
        if (s->destination->on) {
          st = HIGH;
        } else {
          st = LOW;
        }
        for (size_t j = 0; j < s->destination->connections->size; j++) {
          Module *m = find_module(modules, s->destination->connections->items[j]);
          Signal ns = { .source = s->destination, .signal_type = st, .destination = m };
          if (st == LOW) lows++;
          else highs++;
          tracef(signal_print, &ns);
          array_push(q, &ns);
        }
      } else if (s->destination->module_type == CONJUNCTION) {
        s->destination->memory[hash(s->source->name)] = s->signal_type;

        bool all_high = true;
        for (size_t i = 0; i < s->destination->inputs->size; i++) {
          size_t input = *(size_t *) array_get(s->destination->inputs, i);
          all_high = all_high && s->destination->memory[input] == HIGH;
        }

        enum SignalType st = all_high ? LOW : HIGH;

        for (size_t j = 0; j < s->destination->connections->size; j++) {
          Module *m = find_module(modules, s->destination->connections->items[j]);
          Signal ns = { .source = s->destination, .signal_type = st, .destination = m };
          if (st == LOW) lows++;
          else highs++;
          tracef(signal_print, &ns);
          array_push(q, &ns);
        }
      }
      defer:
      free(s);
    }
    array_free(q);
  }

  debug("lows = %llu, highs = %llu\n", lows, highs);

  unsigned long long result = lows * highs;

  for (size_t i = 0; i < modules->size; i++) {
    Module *module = array_get(modules, i);
    free(module->name);
    string_array_free(module->connections);
    free(module->memory);
    array_free(module->inputs);
  }
  array_free(modules);

  return result;
}

long long part2(StringArray *lines) {
  Array *modules = array_new(Module);
  string_array_foreach (char *line, lines) {
    StringArray *split = string_split(line, " -> ");
    char *name = split->items[0];
    if (name[0] == '%' || name[0] == '&') {
      name = name + 1;
    }

    Module module = init_module(name);
    if (line[0] == '%') {
      module.module_type = FLIP_FLOP;
    } else if (line[0] == '&') {
      module.module_type = CONJUNCTION;
    } else {
      module.module_type = BROADCASTER;
    }
    array_push(modules, &module);

    string_array_free(split);
  }
  debugf(array_module_print, modules);

  string_array_foreach (char *line, lines) {
    StringArray *split = string_split(line, " -> ");
    char *name = split->items[0];
    if (name[0] == '%' || name[0] == '&') {
      name = name + 1;
    }

    StringArray *csplit = string_split(split->items[1], ", ");

    Module *module = find_module(modules, name);
    string_array_foreach(char *connection, csplit) {
      Module *c = find_module(modules, connection);
      if (c == nullptr) {
        Module output = init_module(connection);
        array_push(modules, &output);
      }
      string_array_push(module->connections, connection);
    }

    string_array_free(csplit);
    string_array_free(split);
  }
  debugf(array_module_print, modules);

  size_t conjunctions = 4;
  for (size_t i = 0; i < modules->size; i++) {
    Module *m = array_get(modules, i);
    for (size_t j = 0; j < m->connections->size; j++) {
      Module *c = find_module(modules, m->connections->items[j]);
      if (c->module_type == CONJUNCTION) {
        array_push_rval(c->inputs, hash(m->name));
      }
    }
  }
  debugf(array_module_print, modules);
  debug("conjunctions: %zu\n", conjunctions);

  bool done[26*27] = {};
  Array *cycles = array_new(long long);

  long long cycle = 1;
  while (cycles->size < conjunctions) {
    Array *q = array_new(Signal);
    array_push(q, &(Signal) { .source = nullptr, .signal_type = LOW, .destination = find_module(modules, "broadcaster") });
    while (q->size > 0) {
      Signal *s = array_shift(q);
      if (s->destination->module_type == OUTPUT) {
        goto defer;
      } else if (s->destination->module_type == BROADCASTER) {
        for (size_t j = 0; j < s->destination->connections->size; j++) {
          Module *m = find_module(modules, s->destination->connections->items[j]);
          array_push(q, &(Signal) { .source = s->destination, .signal_type = LOW, .destination = m });
        }
      } else if (s->destination->module_type == FLIP_FLOP) {
        if (s->signal_type == HIGH) {
          goto defer;
        }
        s->destination->on = !s->destination->on;
        enum SignalType st;
        if (s->destination->on) {
          st = HIGH;
        } else {
          st = LOW;
        }
        for (size_t j = 0; j < s->destination->connections->size; j++) {
          Module *m = find_module(modules, s->destination->connections->items[j]);
          array_push(q, &(Signal) { .source = s->destination, .signal_type = st, .destination = m });
        }
      } else if (s->destination->module_type == CONJUNCTION) {
        s->destination->memory[hash(s->source->name)] = s->signal_type;

        bool all_high = true;
        for (size_t i = 0; i < s->destination->inputs->size; i++) {
          size_t input = *(size_t *) array_get(s->destination->inputs, i);
          all_high = all_high && s->destination->memory[input] == HIGH;
        }

        enum SignalType st = all_high ? LOW : HIGH;
        if (st == HIGH && (
          strncmp(s->destination->name, "vg", 2) == 0 ||
          strncmp(s->destination->name, "kp", 2) == 0 ||
          strncmp(s->destination->name, "gc", 2) == 0 ||
          strncmp(s->destination->name, "tx", 2) == 0
        )) {
          debug("all high for %s at %lld (%zu)\n", s->destination->name, cycle, cycles->size);
          int h = hash(s->destination->name);
          if (!done[h]) {
            array_push_rval(cycles, cycle);
            done[h] = true;
          }
        }

        for (size_t j = 0; j < s->destination->connections->size; j++) {
          Module *m = find_module(modules, s->destination->connections->items[j]);
          array_push(q, &(Signal) { .source = s->destination, .signal_type = st, .destination = m });
        }
      }
      defer:
      free(s);
    }
    cycle++;
    array_free(q);
  }

  debugf(ll_array_print, cycles);

  long long result = ll_array_get(cycles, 0);
  for (size_t i = 1; i < cycles->size; i++) {
    result = lcm(result, ll_array_get(cycles, i));
  }
  array_free(cycles);

  for (size_t i = 0; i < modules->size; i++) {
    Module *module = array_get(modules, i);
    free(module->name);
    string_array_free(module->connections);
    free(module->memory);
    array_free(module->inputs);
  }
  array_free(modules);

  return result;
}

int main(void) {
  test_case(day20, part1, sample1, 32000000);
  test_case(day20, part1, sample2, 11687500);
  test_case(day20, part1, data, 681194780);
  test_case(day20, part2, data, 238593356738827);

  return 0;
}
