all: build

build: tema3.c
	gcc -o movies tema3.c -g -Wall -lm -std=c11 -ggdb3

.PHONY: clean
clean: 
	rm -f movies