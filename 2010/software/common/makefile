include ../makeopt

all: console.o comm.o bitmap.o

console.o: console.cpp console.h
	$(CC) -o console.o -c console.cpp $(CFLAGS)
	
comm.o: comm.c comm.h
	$(CC) -o comm.o -c comm.c $(CFLAGS)	
	
bitmap.o: bitmap.cpp bitmap.h
	$(CC) -O3 -o bitmap.o -c bitmap.cpp $(CFLAGS)	

clean:
	rm -f *.c~ *.h~ *.cpp~ *.ini~ *.o makefile~ simulateur
