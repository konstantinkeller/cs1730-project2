CC = g++
DEBUG = -g -O0 -pedantic-errors
CFLAGS = -Wall -std=c++14 -c $(DEBUG)
LFLAGS = -Wall -l ncurses $(DEBUG)

all: p2

p2: Editor.o Buffer.o p2.o
	$(CC) $(LFLAGS) -o p2 Editor.o Buffer.o p2.o

Editor.o: Editor.cpp Editor.h
	$(CC) $(CFLAGS) Editor.cpp

Buffer.o: Buffer.cpp Buffer.h
	$(CC) $(CFLAGS) Buffer.cpp

p2.o: p2.cpp
	$(CC) $(CFLAGS) p2.cpp

.PHONY: clean
clean:
	rm -rf *.o
	rm -rf p2
