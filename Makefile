all: czy dzy

czy: czy.c
	gcc czy.c -o czy

dzy: dzy.c
	gcc dzy.c -o dzy

clean:
	rm -rf czy dzy
