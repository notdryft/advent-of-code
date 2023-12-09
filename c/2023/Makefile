cc = gcc
cflags = -O3 -Wall -W

rm = @rm
rmflags = -vf

vg = valgrind
vflags = --leak-check=full

topdir = .
srcdir = $(topdir)/src
objdir = $(topdir)/obj
libdir = $(topdir)/lib
bindir = $(topdir)/bin

days = 9

all: $(patsubst %,day%,$(shell seq 1 1 $(days)))

$(objdir)/%.o: $(srcdir)/%.c 
	$(cc) -c -fPIC -o$@ $^ $(cflags)

$(libdir)/libaoc.so: $(objdir)/array.o $(objdir)/math.o $(objdir)/string.o
	$(cc) -shared -o$@ $^

%_test: $(srcdir)/%_test.c $(libdir)/libaoc.so
	$(cc) -o$(bindir)/$@ -L$(libdir) -laoc $^ $(clags)

day%: $(srcdir)/day%.c $(libdir)/libaoc.so
	$(cc) -o$(bindir)/$@ -L$(libdir) -laoc $^ $(cflags)

mem-day%: day%
	$(vg) $(bindir)/$^ $(vflags)

clean:
	$(rm) $(rmflags) \
		$(objdir)/*.o \
		$(libdir)/*.{a,dylib,so}

mrproper: clean
	$(rm) $(rmflags) \
		$(bindir)/{*_test,day*} \
