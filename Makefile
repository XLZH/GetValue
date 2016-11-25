# Makefile for Getvalue
CC = gcc
CFLAGS = -std=c99 -O3
OBJECT = Bdg.o Region.o Value.o main.o

EXEC = getvalue
all: $(EXEC)

getvalue: $(OBJECT)
	$(CC) $(CFLAGS) $(OBJECT) -o $@

Bdg.o: Share.h
Region.o: Share.h
Value.o: Share.h
main.o: Share.h

clean:
	rm -f  $(OBJECT)

.PHONY : clean


