CFLAGS=-c -std=c99

all: main.o io_functions.o edit_text.o
	gcc main.o io_functions.o edit_text.o -o cw

main.o: main.c 
	gcc $(CFLAGS) main.c

io_functions.o: io_functions.c 
	gcc $(CFLAGS) io_functions.c

edit_text.o: edit_text.c 
	gcc $(CFLAGS) edit_text.c

run:
	./cw

clean:
	rm -rf *.o && rm -rf ./cw
