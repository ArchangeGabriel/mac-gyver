include ../makeopt

all: make_dep interface.o
	$(CC) -o ../simul ../common/console.o ../common/comm.o sdl.o parse_params.o object.o motor.o captor.o camera.o robot.o server.o keycontrol.o io.o pic/usb.o pic/io1.o pic/pic_main1.o pic/io2.o pic/pic_main2.o the_cup/the_elements.o the_cup/the_playground.o simulateur.o interface.o $(LDFLAGS)
	rm -f ../params.ini
	ln ../ini/params.ini ../params.ini
  
make_dep:
	make -C ../common
	make -C pic
	make -C the_cup

sdl.o: sdl.cpp sdl.h
	$(CC) -o sdl.o -c sdl.cpp $(CFLAGS)

server.o: server.cpp server.h
	$(CC) -o server.o -c server.cpp $(CFLAGS)	

io.o: io.cpp io.h
	$(CC) -o io.o -c io.cpp $(CFLAGS)	

keycontrol.o: keycontrol.c keycontrol.h sdl.o server.o io.o
	$(CC) -o keycontrol.o -c keycontrol.c $(CFLAGS)
		
parse_params.o: parse_params.cpp parse_params.h
	$(CC) -o parse_params.o -c parse_params.cpp $(CFLAGS)
	
object.o: sdl.o object.cpp object.h
	$(CC) -o object.o -c object.cpp $(CFLAGS)

captor.o: captor.cpp captor.h object.o parse_params.o sdl.o
	$(CC) -o captor.o -c captor.cpp $(CFLAGS)
	
motor.o: motor.cpp motor.h
	$(CC) -o motor.o -c motor.cpp $(CFLAGS)
	
camera.o: camera.cpp camera.h
	$(CC) -o camera.o -c camera.cpp $(CFLAGS)

robot.o: object.o motor.o captor.o camera.o robot.cpp robot.h
	$(CC) -o robot.o -c robot.cpp $(CFLAGS)

simulateur.o: simulateur.cpp simulateur.h robot.o parse_params.o keycontrol.o
	$(CC) -o simulateur.o -c simulateur.cpp $(CFLAGS)

interface.o: interface.cpp simulateur.o
	$(CC) -o interface.o -c interface.cpp $(CFLAGS)	

clean:
	rm -f *.c~ *.h~ *.cpp~ *.ini~ *.o makefile~ simulateur
	make -s -C pic clean
