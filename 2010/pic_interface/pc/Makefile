CC = g++
DEBUG = -g -O2 
LIB = -lusb
INCLUDE = ../protocole
sdcc_PIC_TYPE = 18f4550
lkr_PIC_TYPE = 18f4550

application_hex_OBJECTS = main.o usbdevicemoteurs.o pic_interface.o usbdeviceinout.o

application_hex_SOURCES = main.cpp usbdevicemoteurs.cpp pic_interface.cpp usbdeviceinout.cpp

COMPILE = $(CC) $(DEBUG) -I$(INCLUDE) -c
LINK = $(CC) -o  host $(DEBUG) $(LIB)

all:
	$(COMPILE) -c main.cpp
	$(COMPILE) -c usbdevicemoteurs.cpp
	$(COMPILE) -c usbdeviceinout.cpp
	$(COMPILE) -c pic_interface.cpp
	$(LINK) $(application_hex_OBJECTS)

clean:
	rm -f *.o host
