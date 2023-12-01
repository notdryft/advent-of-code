cc = gcc
cflags = -O2 -Wall -W 

ar = ar

rm = @rm
rmflags = -vf

vg = valgrind
vflags = --leak-check=full

topdir = .
srcdir = $(topdir)/src
objdir = $(topdir)/obj
libdir = $(topdir)/lib
bindir = $(topdir)/bin

all: day1

$(objdir)/array.o: $(srcdir)/array.c
	$(cc) -c -o$@ $< $(cflags)

$(libdir)/libarray.a: $(objdir)/array.o
	$(ar) -rc $@ $^

day1: $(srcdir)/day1.c $(libdir)/libarray.a
	$(cc) -o$(bindir)/$@ -L$(libdir) -larray $^ $(cflags)

mem-day1: day1
	$(vg) $(bindir)/$< ($vflags)

clean:
	$(rm) $(rmflags) \
		$(objdir)/*.o \
		$(libdir)/*.a

mrproper: clean
	$(rm) $(rmflags) \
		$(bindir)/day*