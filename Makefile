CFLAGS=-c -std=c99

all: main.o io_functions.o
	gcc main.o io_functions.o -o main

main.o: main.c
	gcc $(CFLAGS) main.c

io_functions.o: io_functions.c
	gcc $(CFLAGS) io_functions.c

run:
	./main

clean:
	rm -rf *.o && rm -rf *.out
