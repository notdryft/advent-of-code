cc = gcc-14
cflags = -O5 -pedantic -std=c2x -Wall -Werror -Wextra -Wvla
#cflags += -g
#cflags += -Wpadded

rm = @rm
rmflags = -rfv

vg = valgrind
vflags = --leak-check=full --leak-resolution=high --show-leak-kinds=all --track-origins=yes --vgdb=no

topdir = .
bindir = $(topdir)/bin
objdir = $(topdir)/obj
srcdir = $(topdir)/src

libaocdir = $(topdir)/../libaoc
libaoc = -iquote $(libaocdir)/include -L$(libaocdir)/lib -laoc
libext = dylib

brewdir = $(shell brew --prefix)
brewflags = -I$(brewdir)/include -L$(brewdir)/lib

arch := $(shell uname -s)
ifeq ($(arch),Linux)
	cc = gcc
	libaoc += -Wl,-rpath=$(libaocdir)/lib
	libext = so
endif

year = 2024
days = 14

cflags += -DAOC_YEAR=$(year)

all: $(patsubst %,day%,$(shell seq 1 1 $(days)))

day%: $(srcdir)/day%.c
	$(cc) $(libaoc) -lm -lquadmath -o$(bindir)/$@ $< $(cflags) $(CMDLINE_FLAGS)

day4: $(srcdir)/day4.c
	$(cc) $(brewflags) $(libaoc) -lcrypto -lm -lquadmath -lssl -o$(bindir)/$@ $< $(cflags) $(CMDLINE_FLAGS)

mem-day%: day%
	$(vg) $(vflags) $(bindir)/$^

.PHONY: clean mrproper

clean:
	$(rm) $(rmflags) \
		$(objdir)/*.o

mrproper: clean
	$(rm) $(rmflags) \
		$(bindir)/day*
