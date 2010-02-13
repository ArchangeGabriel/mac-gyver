include ../../makeopt

all: pic_main1.o pic_main2.o
	
usb.o: usb.c usb.h usb_simul.c usb_real.c ../../common/simul.h ../../common/comm.h
	$(CC) -o usb.o -c usb.c $(CFLAGS)
		
io1.o: io1.c io1.h io1_simul.c io1_real.c ../../common/simul.h
	$(CC) -o io1.o -c io1.c $(CFLAGS)
			
pic_main1.o: pic_main1.c pic_main1.h usb.o io1.o ../../common/const.h ../../common/simul.h
	$(CC) -o pic_main1.o -c pic_main1.c $(CFLAGS)
	
io2.o: io2.c io1.h io2_simul.c io2_real.c ../../common/simul.h
	$(CC) -o io2.o -c io2.c $(CFLAGS)	

pic_main2.o: pic_main2.c pic_main2.h usb.o io2.o ../../common/const.h ../../common/simul.h
	$(CC) -o pic_main2.o -c pic_main2.c $(CFLAGS)	
	
clean:
	rm -f *.c~ *.h~ *.cpp~ *.ini~ *.o makefile~ simulateur
