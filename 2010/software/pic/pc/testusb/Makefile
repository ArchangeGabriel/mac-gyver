CC = g++
DEBUG = -g -O2 
LIB = -lusb -lpthread
INCLUDE = ../../protocole -I../

application_hex_OBJECTS = main.o usbdevicemoteurs.o pic_interface.o usbdeviceinout.o console.o

COMPILE = $(CC) $(DEBUG) -I$(INCLUDE) -c
LINK = $(CC) -o  testUSB $(DEBUG) $(LIB)

all:
	$(COMPILE) -o main.o -c testUSB.cpp
	$(COMPILE) -c ../usbdevicemoteurs.cpp
	$(COMPILE) -c ../usbdeviceinout.cpp
	$(COMPILE) -c ../pic_interface.cpp
	$(COMPILE) -o console.o -c ../../../common/console.cpp
	$(LINK) $(application_hex_OBJECTS)

clean:
	rm -f *.o testUSB
