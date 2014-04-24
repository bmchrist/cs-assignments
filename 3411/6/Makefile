CC=gcc
CFLAGS=-c -Wall

all: thing2 thing1

thing1: thing1.o chainvp.o
	$(CC) chainvp.o thing1.o -o thing1

thing2: thing2.o chainvp.o
	$(CC) chainvp.o thing2.o -o thing2

%.o: %.c 
		$(CC) -c -o $@ $< $(CFLAGS)

clean:      
		rm -rf chainvp thing2 thing1 *.o
