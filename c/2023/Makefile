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

arch := $(shell uname -s)
ifeq ($(arch),Linux)
	cc = gcc
	libaoc += -Wl,-rpath=$(libaocdir)/lib
	libext = so
endif

days = 25

all: $(patsubst %,day%,$(shell seq 1 1 $(days)))

$(objdir)/%.o: $(srcdir)/%.c
	$(cc) -c -fPIC -o$@ $^ $(cflags)

day%: $(srcdir)/day%.c
	$(cc) $(libaoc) -lm -lquadmath -o$(bindir)/$@ $< $(cflags) $(CMDLINE_FLAGS)

mem-day%: day%
	$(vg) $(vflags) $(bindir)/$^

.PHONY: clean mrproper

clean:
	$(rm) $(rmflags) \
		$(objdir)/*.o

mrproper: clean
	$(rm) $(rmflags) \
		$(bindir)/day*
