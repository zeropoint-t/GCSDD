
#declare the variable
CC=g++

CFLAGS=-c -Wall -Werror -pedantic

OPT=-O3

all: main

main: main.o nodedata.o bintree.o
	$(CC) $(OPT) main.o nodedata.o bintree.o -o main

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp
	
nodedata.o: nodedata.cpp nodedata.h
	$(CC) $(CFLAGS) nodedata.cpp

bintree.o: bintree.cpp bintree.h  
	$(CC) $(CFLAGS) bintree.cpp

clean:
	rm -rf *.o output