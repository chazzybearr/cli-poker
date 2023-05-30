SRCDIR=../src
INCDIR=../include
FLAGS=-Wall -g -std=gnu99

all: poker.o poker_game.o

poker.o: $(SRCDIR)/poker.c $(INCDIR)/poker.h
	gcc $(FLAGS) -c $(SRCDIR)/poker.c

poker_game.o: $(SRCDIR)/poker_game.c $(INCDIR)/poker_game.h
	gcc $(FLAGS) -c $(SRCDIR)/poker_game.c

clean:
	rm *.o