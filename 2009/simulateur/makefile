all: make_src make_pc

make_src:
	make -C src/simul

make_pc:
	make -C src/pc

clean:
	rm -f *.c~ *.h~ *.cpp~ *.ini~ *.o makefile~ bin/*
	rm -f simulateur client params.ini
	make -s -C src/simul clean
	make -s -C src/pic  clean	
	make -s -C src/pc  clean
	make -s -C src/common clean
