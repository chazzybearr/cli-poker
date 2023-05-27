SRCDIR=../src
INCDIR=../include

all: poker.o poker_game.o

poker.o: $(SRCDIR)/poker.c $(INCDIR)/poker.h
	gcc -Wall -Werror -g -std=gnu99 -c $(SRCDIR)/poker.c

poker_game.o: $(SRCDIR)/poker_game.c $(INCDIR)/poker_game.h
	gcc -Wall -Werror -g -std=gnu99 -c $(SRCDIR)/poker_game.c

clean:
	rm *.o