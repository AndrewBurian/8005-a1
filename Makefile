CC=gcc
LIBS=-lpthread -lgmp

all: bin/a1

clean:
	rm obj/*.o bin/*

bin/a1: obj/main.o obj/primedecompose.o
	$(CC) -o bin/a1 obj/main.o obj/primedecompose.o $(LIBS)

obj/main.o: src/main.c
	$(CC) -c -o obj/main.o src/main.c

obj/primedecompose.o: src/primedecompose.c src/primedecompose.h
	$(CC) -c -o obj/primedecompose.o src/primedecompose.c

