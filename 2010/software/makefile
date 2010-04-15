include makeopt

all: do_simul do_pc
	echo "done"

do_simul:
	make -C simulateur
	
do_pc:
	make -C pc

clean:
	rm -f *.c~ *.h~ *.cpp~ ini/*.ini~ *.o makefile~
	rm -f simul client params.ini
	make -s -C pc clean
	make -s -C pc/tools clean
	make -s -C simulateur clean
	make -s -C common clean

