PORT=56524
CFLAGS= -DPORT=\$(PORT) -g -std=gnu99 -Wall
SRCDIR=src
BUILDDIR=build
INCDIR=include

poker_server:$(SRCDIR)/poker_server.c $(SRCDIR)/poker_game.c $(SRCDIR)/poker.c
	$(MAKE) -C $(BUILDDIR)
	gcc $(CFLAGS) -o poker_server $(SRCDIR)/poker_server.c $(BUILDDIR)/poker.o $(BUILDDIR)/poker_game.o


clean:
	rm poker_server