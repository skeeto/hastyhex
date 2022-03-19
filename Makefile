.POSIX:
CC      = cc
CFLAGS  = -ansi -pedantic -Wall -Wextra -O3 -march=native -funroll-loops
LDFLAGS = -s
LDLIBS  = 
PREFIX  = /usr/local

all: hastyhex$(EXE)

hastyhex$(EXE): hastyhex.c
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ hastyhex.c $(LDLIBS)

install: hastyhex$(EXE)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	mkdir -p $(DESTDIR)$(PREFIX)/share/man/man1
	install -m 755 hastyhex$(EXE) $(DESTDIR)$(PREFIX)/bin
	gzip <hastyhex.1 >$(DESTDIR)$(PREFIX)/share/man/man1/hastyhex.1.gz

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/hastyhex$(EXE)
	rm -f $(DESTDIR)$(PREFIX)/share/man/man1/hastyhex.1.gz

clean:
	rm -f hastyhex$(EXE)
