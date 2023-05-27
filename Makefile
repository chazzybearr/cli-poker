PORT=56524
CFLAGS= -DPORT=\$(PORT) -g -std=gnu99 -Wall
SRCDIR=src
BUILDDIR=build
INCDIR=include

poker_server: $(SRCDIR)/poker_server.c $(BUILDDIR)/poker_game.o $(BUILDDIR)/poker.o include/poker.h include/poker_game.h src/poker.c src/poker_game.c
	$(MAKE) -C $(BUILDDIR)
	gcc $(CFLAGS) -o poker_server $(SRCDIR)/poker_server.c $(BUILDDIR)/poker.o $(BUILDDIR)/poker_game.o

$(BUILDDIR)/poker_game.o:

$(BUILDDIR)/poker.o:

clean:
	rm poker_server