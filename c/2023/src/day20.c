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
  char name[64];
  enum ModuleType module_type;
  bool on;
  size_t connections_size;
  char connections[10][64];
  int memory[26*27];
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
  if (module->connections_size > 0) {
    printf("-> ");
  } else {
    printf("X");
  }
  for (size_t i = 0; i < module->connections_size; i++) {
    printf("%s", module->connections[i]);
    if (i < module->connections_size - 1) printf(", ");
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

Module *find_or_init_module(Array *modules, char *name) {
  Module *module = find_module(modules, name);
  if (module == nullptr) {
    Module tmp = {};
    strncpy(tmp.name, name, 63);
    array_push(modules, &tmp);

    module = find_module(modules, name);
    module->module_type = OUTPUT;
    module->connections_size = 0;
    for (size_t i = 0; i < 10; i++) {
      memset(module->connections[i], 0, 64);
    }
    memset(module->memory, 0, sizeof(int) * 26 * 27);
    module->inputs = array_new(size_t);
  }

  return module;
}

unsigned long long part1(StringArray *lines) {
  Array *modules = array_new(Module);
  Module *broadcast = find_or_init_module(modules, "broadcaster");
  broadcast->module_type = BROADCASTER;

  for (size_t l = 0; l < lines->size; l++) {
    char *line = lines->items[l];

    if (strncmp(line, "broadcaster", 11) == 0 || line[0] == '%' || line[0] == '&') {
      StringArray *split = string_split(line, " -> ");
      StringArray *csplit = string_split(split->items[1], ", ");
      if (strncmp(line, "broadcaster", 11) == 0) {
        debugf(array_module_print, modules);
        for (size_t i = 0; i < csplit->size; i++) {
          char *name = csplit->items[i];
          strncpy(broadcast->connections[broadcast->connections_size++], name, 64);
          debugf(array_module_print, modules);
        }
      } else if (line[0] == '%') {
        Module *fp = find_or_init_module(modules, split->items[0] + 1);
        fp->module_type = FLIP_FLOP;
        debugf(array_module_print, modules);
        for (size_t i = 0; i < csplit->size; i++) {
          strncpy(fp->connections[fp->connections_size++], csplit->items[i], 64);
          debugf(array_module_print, modules);
        }
      } else if (line[0] == '&') {
        Module *conj = find_or_init_module(modules, split->items[0] + 1);
        conj->module_type = CONJUNCTION;
        debugf(array_module_print, modules);
        for (size_t i = 0; i < csplit->size; i++) {
          strncpy(conj->connections[conj->connections_size++], csplit->items[i], 64);
          find_or_init_module(modules, csplit->items[i]);
          debugf(array_module_print, modules);
        }
      }

      string_array_free(csplit);
      string_array_free(split);
    } else {
      printf("unlucky\n");
      exit(42);
    }
  }

  for (size_t i = 0; i < modules->size; i++) {
    Module *m = array_get(modules, i);
    for (size_t j = 0; j < m->connections_size; j++) {
      Module *c = find_module(modules, m->connections[j]);
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
    array_push(q, &(Signal) { .source = nullptr, .signal_type = LOW, .destination = broadcast });
    while (q->size > 0) {
      Signal *s = array_pop(q);
      if (s->destination->module_type == OUTPUT) {
        free(s);
        continue;
      } else if (s->destination->module_type == BROADCASTER) {
        debugf(signal_print, s);
        lows++;
        for (size_t j = 0; j < s->destination->connections_size; j++) {
          lows++;
          Module *m = find_module(modules, s->destination->connections[j]);
          Signal ns = { .source = s->destination, .signal_type = LOW, .destination = m };
          debugf(signal_print, &ns);
          array_push(q, &ns);
        }
      } else if (s->destination->module_type == FLIP_FLOP) {
        if (s->signal_type == HIGH) {
          free(s);
          continue;
        }
        s->destination->on = !s->destination->on;
        enum SignalType st;
        if (s->destination->on) {
          st = HIGH;
        } else {
          st = LOW;
        }
        for (size_t j = 0; j < s->destination->connections_size; j++) {
          Module *m = find_module(modules, s->destination->connections[j]);
          Signal ns = { .source = s->destination, .signal_type = st, .destination = m };
          if (st == LOW) lows++;
          else highs++;
          debugf(signal_print, &ns);
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

        for (size_t j = 0; j < s->destination->connections_size; j++) {
          Module *m = find_module(modules, s->destination->connections[j]);
          Signal ns = { .source = s->destination, .signal_type = st, .destination = m };
          if (st == LOW) lows++;
          else highs++;
          debugf(signal_print, &ns);
          array_push(q, &ns);
        }
      }
      free(s);
    }
    array_free(q);
  }

  debug("lows = %llu, highs = %llu\n", lows, highs);

  unsigned long long result = lows * highs;

  for (size_t i = 0; i < modules->size; i++) {
    Module *module = array_get(modules, i);
    array_free(module->inputs);
  }
  array_free(modules);

  return result;
}

long long part2(StringArray *lines) {
  Array *modules = array_new(Module);
  Module *broadcast = find_or_init_module(modules, "broadcaster");
  broadcast->module_type = BROADCASTER;

  for (size_t l = 0; l < lines->size; l++) {
    char *line = lines->items[l];

    if (strncmp(line, "broadcaster", 11) == 0 || line[0] == '%' || line[0] == '&') {
      StringArray *split = string_split(line, " -> ");
      StringArray *csplit = string_split(split->items[1], ", ");
      if (strncmp(line, "broadcaster", 11) == 0) {
        debugf(array_module_print, modules);
        for (size_t i = 0; i < csplit->size; i++) {
          char *name = csplit->items[i];
          strncpy(broadcast->connections[broadcast->connections_size++], name, 64);
          debugf(array_module_print, modules);
        }
      } else if (line[0] == '%') {
        Module *fp = find_or_init_module(modules, split->items[0] + 1);
        fp->module_type = FLIP_FLOP;
        debugf(array_module_print, modules);
        for (size_t i = 0; i < csplit->size; i++) {
          strncpy(fp->connections[fp->connections_size++], csplit->items[i], 64);
          debugf(array_module_print, modules);
        }
      } else if (line[0] == '&') {
        Module *conj = find_or_init_module(modules, split->items[0] + 1);
        conj->module_type = CONJUNCTION;
        debugf(array_module_print, modules);
        for (size_t i = 0; i < csplit->size; i++) {
          strncpy(conj->connections[conj->connections_size++], csplit->items[i], 64);
          find_or_init_module(modules, csplit->items[i]);
          debugf(array_module_print, modules);
        }
      }

      string_array_free(csplit);
      string_array_free(split);
    } else {
      printf("unlucky\n");
      exit(42);
    }
  }

  size_t conjunctions = 4;
  for (size_t i = 0; i < modules->size; i++) {
    Module *m = array_get(modules, i);
    for (size_t j = 0; j < m->connections_size; j++) {
      Module *c = find_module(modules, m->connections[j]);
      if (c->module_type == CONJUNCTION) {
        array_push_rval(c->inputs, hash(m->name));
      }
    }
  }
  debugf(array_module_print, modules);
  debug("conjunctions: %zu\n", conjunctions);

  bool done[26*27];
  memset(done, 0, 26*27*sizeof(bool));
  Array *cycles = array_new(long long);

  long long cycle = 0;
  while (cycles->size < conjunctions) {
    Array *q = array_new(Signal);
    array_push(q, &(Signal) { .source = nullptr, .signal_type = LOW, .destination = broadcast });
    while (q->size > 0) {
      Signal *s = array_pop(q);
      if (s->destination->module_type == OUTPUT) {
        free(s);
        continue;
      } else if (s->destination->module_type == BROADCASTER) {
        for (size_t j = 0; j < s->destination->connections_size; j++) {
          Module *m = find_module(modules, s->destination->connections[j]);
          array_push(q, &(Signal) { .source = s->destination, .signal_type = LOW, .destination = m });
        }
      } else if (s->destination->module_type == FLIP_FLOP) {
        if (s->signal_type == HIGH) {
          free(s);
          continue;
        }
        s->destination->on = !s->destination->on;
        enum SignalType st;
        if (s->destination->on) {
          st = HIGH;
        } else {
          st = LOW;
        }
        for (size_t j = 0; j < s->destination->connections_size; j++) {
          Module *m = find_module(modules, s->destination->connections[j]);
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

        for (size_t j = 0; j < s->destination->connections_size; j++) {
          Module *m = find_module(modules, s->destination->connections[j]);
          array_push(q, &(Signal) { .source = s->destination, .signal_type = st, .destination = m });
        }
      }
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

  for (size_t i = 0; i < modules->size; i++) {
    Module *module = array_get(modules, i);
    array_free(module->inputs);
  }
  array_free(modules);

  return result;
}

int main(void) {
  //test_case(day20, part1, sample1, 32000000);
  //test_case(day20, part1, sample2, 11687500);
  //test_case(day20, part1, data, 681194780); // random result???
  test_case(day20, part2, data, 238593356738827);

  return 0;
}
