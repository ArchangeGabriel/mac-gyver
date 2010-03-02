CC = g++
DEBUG = -g -O2 
LIB = -lusb -lpthread
INCLUDE = ../../protocole
OUTPUT = USBscruter

application_hex_OBJECTS = main.o

COMPILE = $(CC) $(DEBUG) -I$(INCLUDE) -c
LINK = $(CC) -o  $(OUTPUT) $(DEBUG) $(LIB)

all:
	$(COMPILE) -o main.o -c scruter.cpp
	$(LINK) $(application_hex_OBJECTS)

clean:
	rm -f *.o $(OUTPUT)
