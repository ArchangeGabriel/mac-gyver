include ../makeopt

all: the_elements.o the_playground.o 

the_elements.o: the_elements.cpp the_elements.h colors.h
	$(CC) -o the_elements.o -c the_elements.cpp $(CFLAGS)
	
the_playground.o: the_playground.cpp the_playground.h the_elements.o
	$(CC) -o the_playground.o -c the_playground.cpp $(CFLAGS)
		
clean:
	rm -f *.c~ *.h~ *.cpp~ *.ini~ *.o makefile~ simulateur
