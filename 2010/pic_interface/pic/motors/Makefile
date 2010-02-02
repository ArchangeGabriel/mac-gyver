SDCC = ../compiler/tools/bin/sdcc
DEBUG = --optimize-goto --denable-peeps --obanksel=9 --opt-code-size --optimize-cmp --optimize-df --fstack 
LIBPUF = ../compiler/lib/libpuf.lib
INCLUDE = ../include -I../protocole
sdcc_PIC_TYPE = 18f4550
lkr_PIC_TYPE = 18f4550

application_hex_OBJECTS = application_iface.o \
	main.o usb_descriptors.o\
	ep1.o ep2.o codeuses.o alim.o

application_hex_SOURCES = usb_descriptors.h  \
                          application_iface.c main.c \
                          usb_descriptors.c codeuses.c\
			  ep1.c ep2.c alim.c


COMPILE = $(SDCC) -mpic16 $(DEBUG) -p$(sdcc_PIC_TYPE) -I$(INCLUDE) -c
LINK = $(SDCC) -o  motors $(DEBUG) $(LDFLAGS) --no-crt -Wl-s$(lkr_PIC_TYPE).lkr,-m \
	-mpic16 -p$(sdcc_PIC_TYPE) -l$(LIBPUF) -llibio$(sdcc_PIC_TYPE).lib -llibc18f.lib

all:
	$(COMPILE) -c application_iface.c
	$(COMPILE) -c main.c
	$(COMPILE) -c usb_descriptors.c
	$(COMPILE) -c codeuses.c
	$(COMPILE) -c alim.c
	$(COMPILE) -c ep1.c
	$(COMPILE) -c ep2.c
	$(LINK) $(application_hex_OBJECTS)

clean:
	rm -f *.o *.lst *.rel *.cod *.map *.asm motors.hex
