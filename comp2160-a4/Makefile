main: main.c ObjectManager.o
	clang++ -Wall main.c ObjectManager.o -o $@

ObjectManager.o: ObjectManager.c ObjectManager.h
	clang++ -Wall -c ObjectManager.c -o $@