CC=gcc
CFLAGS=-c -Wall

all: rclient1 rclient2 server

server: server.o
	$(CC) server.o -o server

rclient1: client.o userprog1.o
	$(CC) userprog1.o client.o -o rclient1

rclient2: client.o userprog2.o
	$(CC) userprog2.o client.o -o rclient2

%.o: %.c 
		$(CC) -c -o $@ $< $(CFLAGS)

clean:      
		rm -rf server rclient1 rclient2 *.o
