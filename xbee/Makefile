CC=g++
LFLAGS=-lxbee -lpthread -lrt 
LIBFLAGS=-L/usr/lib/ -I/usr/include/

all: terminal terminal_raw terminal_directional

cpptest:
	$(CC) -o lol XBEETest.cpp XBEERadio.cpp $(LFLAGS) $(LIBFLAGS) -std=c++11

terminal_directional:
	$(CC) terminal_directional.c -o terminaldirectional $(LIBFLAGS) $(LFLAGS)

terminal_raw:
	$(CC) terminal_raw.c -o terminalraw $(LIBFLAGS) $(LFLAGS)

terminal:
	$(CC) terminal_input.c -o terminal $(LIBFLAGS) $(LFLAGS)

clean:
	rm -rf terminal terminalraw terminaldirectional
