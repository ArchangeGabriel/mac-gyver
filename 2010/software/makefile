include makeopt

all: simulateur pc

simulateur:
	make -C simulateur
	
pc:
	make -C pc

clean:
	rm -f *.c~ *.h~ *.cpp~ ini/*.ini~ *.o makefile~
	rm -f simulateur client params.ini
	make -s -C pc clean
	make -s -C simulateur clean
	make -s -C src/the_cup clean
	make -s -C src/simul clean
	make -s -C src/pic  clean	
	make -s -C src/pc  clean
	make -s -C src/common clean
