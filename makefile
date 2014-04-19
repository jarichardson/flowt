CC = gcc
CFLAGS = -Wall -pedantic
.SUFFIXES: .o .c
.c.o: ; $(CC) -c $(CFLAGS) $*.c
OBJ = init.o boundary.o uvp.o main.o

Flowt: $(OBJ)
	$(CC) $(CFLAGS) -o Flowt $(OBJ) -lm

init.o : init.h
boundary.o : init.h
uvp.o : init.h
main.o : init.h
