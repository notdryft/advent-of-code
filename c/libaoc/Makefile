cc = gcc-14
cflags = -O5 -pedantic -std=c2x -Wall -Werror -Wextra -Wvla
#cflags += -g
#cflags += -Wpadded

libext = dylib

rm = @rm
rmflags = -rfv

vg = valgrind
vflags = --leak-check=full --leak-resolution=high --show-leak-kinds=all --track-origins=yes --vgdb=no

topdir = .
bindir = $(topdir)/bin
objdir = $(topdir)/obj
incdir = $(topdir)/include
libdir = $(topdir)/lib
srcdir = $(topdir)/src/main
testdir = $(topdir)/src/test

libaoc = -iquote $(incdir) -L$(libdir) -laoc

arch := $(shell uname -s)
ifeq ($(arch),Linux)
	cc = gcc
	libaoc += -Wl,-rpath=$(libdir)
	libext = so
endif

all: $(libdir)/libaoc.$(libext)

$(objdir)/%.o: $(srcdir)/%.c
	$(cc) -c -fPIC -iquote $(incdir) -o$@ $^ $(cflags)

$(libdir)/libaoc.$(libext): $(objdir)/algos.o $(objdir)/array.o $(objdir)/commons.o $(objdir)/map.o $(objdir)/math.o $(objdir)/string.o $(objdir)/time.o
	$(cc) -shared -o$@ $^

%_test: $(testdir)/%.c $(libdir)/libaoc.$(libext)
	$(cc) $(libaoc) -o$(bindir)/$@ $^ $(cflags) $(CMDLINE_FLAGS)

mem-%_test: %_test
	$(vg) $(bindir)/$^ $(vflags)

.PHONY: clean mrproper

clean:
	$(rm) $(rmflags) \
		$(objdir)/*.o \
		$(libdir)/*.{dylib,so}

mrproper: clean
	$(rm) $(rmflags) \
		$(bindir)/*_test
