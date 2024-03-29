cc = gcc-13
cflags = -O5 -pedantic -std=c2x -Wall -Werror -Wextra -Wvla
#cflags += -g
#cflags += -Wpadded

rm = @rm
rmflags = -rfv

vg = valgrind
vflags = --leak-check=full --leak-resolution=high --show-leak-kinds=all --track-origins=yes --vgdb=no

topdir = .
srcdir = $(topdir)/src
objdir = $(topdir)/obj
libdir = $(topdir)/lib
bindir = $(topdir)/bin

arch := $(shell uname -s)
ifeq ($(arch),Linux)
	cc = gcc
	cflags += -Wl,-R$(libdir)
endif

days = 25

all: $(patsubst %,day%,$(shell seq 1 1 $(days)))

$(objdir)/%.o: $(srcdir)/%.c
	$(cc) -c -fPIC -o$@ $^ $(cflags)

$(libdir)/libaoc.so: $(objdir)/algos.o $(objdir)/array.o $(objdir)/commons.o $(objdir)/map.o $(objdir)/math.o $(objdir)/string.o $(objdir)/time.o
	$(cc) -shared -o$@ $^

%_test: $(srcdir)/%_test.c $(libdir)/libaoc.so
	$(cc) -o$(bindir)/$@ -L$(libdir) -laoc $^ $(cflags) $(CMDLINE_FLAGS)

mem-%_test: %_test
	$(vg) $(bindir)/$^ $(vflags)

day%: $(srcdir)/day%.c $(libdir)/libaoc.so
	$(cc) -o$(bindir)/$@ -L$(libdir) -laoc -lm -lquadmath $< $(cflags) $(CMDLINE_FLAGS)

mem-day%: day%
	$(vg) $(vflags) $(bindir)/$^

clean:
	$(rm) $(rmflags) \
		$(objdir)/*.o \
		$(libdir)/*.{a,dylib,so}

mrproper: clean
	$(rm) $(rmflags) \
		$(bindir)/{*_test,day*} \
