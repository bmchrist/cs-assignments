CC=gcc
CFLAGS=-c -Wall

all: thing2 thing1

thing1: thing1.o chainvp.o
	$(CC) chainvp.o thing1.o -o Thing1

thing2: thing2.o chainvp.o
	$(CC) chainvp.o thing2.o -o Thing2

%.o: %.c 
		$(CC) -c -o $@ $< $(CFLAGS)

clean:      
		rm -rf chainvp Thing2 Thing1 *.o
