

CFLAGS+=-Wall -Wextra -g -std=c99 -O3 -pedantic -Ideps
PREFIX?=/usr/local
BINDIR?=$(PREFIX)/bin

LIBS=-lpthread -lncurses

all: walledin

walledin:
	gcc $(CFLAGS) Run.c RunMainMulti.c -o walledin $(LIBS)

install: walledin
	mkdir -p $(DESTDIR)$(BINDIR)
	cp walledin $(DESTDIR)$(BINDIR)/
	chmod 755 ${DESTDIR}${BINDIR}/walledin
