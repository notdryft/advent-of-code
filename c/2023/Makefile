cc = gcc-13
cflags = -O5 -pedantic -std=c2x -Wall -Werror -Wextra -Wvla
#cflags += -Wpadded

rm = @rm
rmflags = -vf

vg = valgrind
vflags = --leak-check=full --leak-resolution=high --show-leak-kinds=all --track-origins=yes --vgdb=no

topdir = .
srcdir = $(topdir)/src
objdir = $(topdir)/obj
libdir = $(topdir)/lib
bindir = $(topdir)/bin

arch := $(shell uname -s)
ifeq ($(arch),Linux)
	cflags += -Wl,-R$(libdir)
endif

days = 25

all: $(patsubst %,day%,$(shell seq 1 1 $(days)))

$(objdir)/%.o: $(srcdir)/%.c
	$(cc) -c -fPIC -o$@ $^ $(cflags)

$(libdir)/libaoc.so: $(objdir)/algos.o $(objdir)/array.o $(objdir)/map.o $(objdir)/math.o $(objdir)/string.o
	$(cc) -shared -o$@ $^

%_test: $(srcdir)/%_test.c $(libdir)/libaoc.so
	$(cc) -o$(bindir)/$@ -L$(libdir) -laoc $^ $(cflags)

mem-%_test: %_test
	$(vg) $(bindir)/$^ $(vflags)

day%: $(srcdir)/day%.c $(libdir)/libaoc.so
	$(cc) -o$(bindir)/$@ -L$(libdir) -laoc -lm -lquadmath $< $(cflags)

mem-day%: day%
	$(vg) $(vflags) $(bindir)/$^

clean:
	$(rm) $(rmflags) \
		$(objdir)/*.o \
		$(libdir)/*.{a,dylib,so}

mrproper: clean
	$(rm) $(rmflags) \
		$(bindir)/{*_test,day*} \
