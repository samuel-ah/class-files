main: main.c main.h container.o
	clang -Wall main.c container.o -o $@

container.o: container.c container.h
	clang -Wall -c container.c -o $@