CC=g++
LFLAGS=-lxbee -lpthread -lrt -lncurses 
LIBFLAGS=-L/usr/lib/ -I/usr/include/

all: ncurses_test

run: ncurses_test
	./ncurses_test.o

ncurses_test: 
	$(CC) ncurses_test.cpp -o ncurses_test.o $(LFLAGS) $(LIBFLAGS)

clean:
	rm -rf *.o
