include ../makeopt

all: console.o comm.o

console.o: console.cpp console.h
	$(CC) -o console.o -c console.cpp $(CFLAGS)
	
comm.o: comm.c comm.h
	$(CC) -o comm.o -c comm.c $(CFLAGS)	

clean:
	rm -f *.c~ *.h~ *.cpp~ *.ini~ *.o makefile~ simulateur
