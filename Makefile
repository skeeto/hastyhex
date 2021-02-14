.POSIX:
CC      = cc
CFLAGS  = -ansi -pedantic -Wall -Wextra -O3 -march=native -funroll-loops
LDFLAGS =
LDLIBS  = 

all: hastyhex$(EXE)

hastyhex$(EXE): hastyhex.c
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ hastyhex.c $(LDLIBS)

clean:
	rm -f hastyhex$(EXE)
