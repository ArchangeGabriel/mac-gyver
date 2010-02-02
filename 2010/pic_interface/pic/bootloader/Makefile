SDCC = ../compiler/tools/bin/sdcc
DEBUG = --optimize-goto --denable-peeps --obanksel=9 --opt-code-size --optimize-cmp --optimize-df --fstack 
LIBPUF = ../compiler/lib/libpuf.lib
INCLUDE = ../include
sdcc_PIC_TYPE = 18f4550
lkr_PIC_TYPE = 18f4550

bootloader_hex_OBJECTS = application_iface.o \
	boot_main.o boot_iface.o usb.o \
	usb_descriptors.o ep0.o ep1.o \
	ep2.o flash.o config.o \
	interrupt_iface.o alim.o

bootloader_hex_SOURCES = application_iface.c boot_main.c boot_iface.c \
                         usb.c usb_descriptors.c ep0.c ep1.c ep2.c flash.c \
                         config.c interrupt_iface.c usb.h usb_descriptors.h ep0.h \
                         ep1.h ep2.h application_iface.h \
                         usb_std_req.h common_types.h \
                         debug.h flash.h boot_iface.h config.h


#COMPILE = $(SDCC) -mpic16 $(DEBUG) -p$(sdcc_PIC_TYPE) -I$(INCLUDE) -c
#LINK = $(SDCC) -o  application $(DEBUG) $(LDFLAGS) --no-crt -Wl-s$(lkr_PIC_TYPE).lkr,-m \
#	-mpic16 -p$(sdcc_PIC_TYPE) -l$(LIBPUF) -llibio$(sdcc_PIC_TYPE).lib -llibc18f.lib

COMPILE = $(SDCC) -mpic16 $(DEBUG) -p$(sdcc_PIC_TYPE) -I$(INCLUDE) -c
LINK = $(SDCC) -o bootloader $(DEBUG) $(LDFLAGS) --use-crt=crt0.o -Wl-s$(lkr_PIC_TYPE).lkr,-m \
	-mpic16 -p$(sdcc_PIC_TYPE) -llibio$(sdcc_PIC_TYPE).lib -llibc18f.lib

all:
	$(COMPILE) -c application_iface.c
	$(COMPILE) -c boot_main.c
	$(COMPILE) -c boot_iface.c
	$(COMPILE) -c usb.c
	$(COMPILE) -c usb_descriptors.c
	$(COMPILE) -c alim.c
	$(COMPILE) -c ep0.c
	$(COMPILE) -c ep1.c
	$(COMPILE) -c ep2.c
	$(COMPILE) -c flash.c
	$(COMPILE) -c config.c
	$(COMPILE) -c interrupt_iface.c
	$(LINK) $(bootloader_hex_OBJECTS)

clean:
	rm -f *.o *.lst *.rel *.cod *.map *.asm bootloader.hex
