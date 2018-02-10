CC=gcc
LFLAGS=-lxbee -lpthread -lrt 
LIBFLAGS=-L/usr/lib/ -I/usr/include/

all: terminal

terminal:
	$(CC) terminal_input.c -o terminal $(LIBFLAGS) $(LFLAGS)

clean:
	rm -rf terminal 
