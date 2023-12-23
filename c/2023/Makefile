cc = gcc-13
cflags = -O3 -std=c2x -Wall -W

rm = @rm
rmflags = -vf

vg = valgrind
vflags = --leak-check=full --show-leak-kinds=all

topdir = .
srcdir = $(topdir)/src
objdir = $(topdir)/obj
libdir = $(topdir)/lib
bindir = $(topdir)/bin

days = 23

all: $(patsubst %,day%,$(shell seq 1 1 $(days)))

$(objdir)/%.o: $(srcdir)/%.c 
	$(cc) -c -fPIC -o$@ $^ $(cflags)

$(libdir)/libaoc.so: $(objdir)/algos.o $(objdir)/array.o $(objdir)/map.o $(objdir)/math.o $(objdir)/string.o
	$(cc) -shared -o$@ $^

%_test: $(srcdir)/%_test.c $(libdir)/libaoc.so
	$(cc) -o$(bindir)/$@ -L$(libdir) -laoc $^ $(clags)

mem-%_test: %_test
	$(vg) $(bindir)/$^ $(vflags)

day%: $(srcdir)/day%.c $(libdir)/libaoc.so
	$(cc) -o$(bindir)/$@ -L$(libdir) -laoc $^ $(cflags)

mem-day%: day%
	$(vg) $(vflags) $(bindir)/$^

clean:
	$(rm) $(rmflags) \
		$(objdir)/*.o \
		$(libdir)/*.{a,dylib,so}

mrproper: clean
	$(rm) $(rmflags) \
		$(bindir)/{*_test,day*} \
