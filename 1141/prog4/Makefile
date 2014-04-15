CFLAGS=-c -Wall -ansi -pedantic

all: bst

bst: tree.o main.o func.o
	$(CC) func.o tree.o main.o -o bst

main.o: main.c
		$(CC) $(CFLAGS) main.c

tree.o: tree.c
		$(CC) $(CFLAGS) tree.c

func.o: func.c
		$(CC) $(CFLAGS) func.c

clean:
	rm -rf *.o bst

run: bst
	./bst
