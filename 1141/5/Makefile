CFLAGS=-c -Wall -ansi -pedantic

all: huffman

huffman: tree.o huffman.o func.o
	$(CC) func.o tree.o huffman.o -o huffman

%.o: %.c 
		$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf *.o huffman

run: huffman
	./huffman
