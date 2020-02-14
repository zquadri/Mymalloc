all: memgrind
mymalloc.o: mymalloc.c mymalloc.h
	gcc -c mymalloc.c
memgrind: mymalloc.o memgrind.c
	gcc -o memgrind -g memgrind.c mymalloc.o
clean:
	rm -f memgrind mymalloc.o
