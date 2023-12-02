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

all: day1

$(objdir)/%.o: $(srcdir)/%.c 
	$(cc) -c -fPIC -o$@ $^ $(cflags)

$(libdir)/libaoc.so: $(objdir)/array.o $(objdir)/string.o
	$(cc) -shared -o$@ $^

%_test: $(srcdir)/%_test.c $(libdir)/libaoc.so
	$(cc) -o$(bindir)/$@ -L$(libdir) -laoc $^ $(clags)

day1: $(srcdir)/day1.c $(libdir)/libaoc.so
	$(cc) -o$(bindir)/$@ -L$(libdir) -laoc $^ $(cflags)

mem-day1: day1
	$(vg) $(bindir)/$^ $(vflags)

day2: $(srcdir)/day2.c $(libdir)/libaoc.so
	$(cc) -o$(bindir)/$@ -L$(libdir) -laoc $^ $(cflags)

mem-day2: day2
	$(vg) $(bindir)/$^ $(vflags)

clean:
	$(rm) $(rmflags) \
		$(objdir)/*.{dylib,o,so} \
		$(libdir)/*.a

mrproper: clean
	$(rm) $(rmflags) \
		$(bindir)/{*_test,day*} \
