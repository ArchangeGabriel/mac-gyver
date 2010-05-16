include ../../makeopt

all: pic_interface.o usbdeviceinout.o usbdevicemoteurs.o

pic_interface.o: pic_interface.cpp pic_interface.h ../simul/pic_interface.cpp ../../common/simul.h
	$(CC) -o pic_interface.o -c pic_interface.cpp $(CFLAGS)
	
usbdeviceinout.o: usbdeviceinout.cpp usbdeviceinout.h
	$(CC) -o usbdeviceinout.o -c usbdeviceinout.cpp $(CFLAGS)	
	
usbdevicemoteurs.o: usbdevicemoteurs.cpp usbdevicemoteurs.h
	$(CC) -O3 -o usbdevicemoteurs.o -c usbdevicemoteurs.cpp $(CFLAGS)	

clean:
	rm -f *.c~ *.h~ *.cpp~ *.ini~ *.o makefile~
