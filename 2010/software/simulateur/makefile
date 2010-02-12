include ../makeopt

all: make_dep interface.o
	$(CC) -o ../simul common/console.o common/comm.o simulateur/sdl.o simulateur/parse_params.o simulateur/object.o simulateur/motor.o simulateur/captor.o simulateur/camera.o simulateur/robot.o simulateur/server.o simulateur/keycontrol.o io.o pic/usb.o pic/io1.o pic/pic_main1.o pic/io2.o pic/pic_main2.o simulateur/the_cup/the_elements.o simulateur/the_cup/the_playground.o simulateur/simulateur.o simulateur/interface.o $(LDFLAGS)
	rm -f ../params.ini
	ln ../params.ini ini/params.ini
  
make_dep:
	make -C pic
	make -C the_cup

sdl.o: sdl.cpp sdl.h
	$(CC) -o sdl.o -c sdl.cpp $(CFLAGS)

server.o: server.cpp server.h ../common/comm.h
	$(CC) -o server.o -c server.cpp $(CFLAGS)	

io.o: io.cpp io.h
	$(CC) -o io.o -c io.cpp $(CFLAGS)	

keycontrol.o: keycontrol.c keycontrol.h sdl.o server.o io.o pic/simul.h robot.h ../common/const.h
	$(CC) -o keycontrol.o -c keycontrol.c $(CFLAGS)
		
parse_params.o: parse_params.cpp parse_params.h
	$(CC) -o parse_params.o -c parse_params.cpp $(CFLAGS)
	
object.o: sdl.o object.cpp object.h ../common/lin_alg.h
	$(CC) -o object.o -c object.cpp $(CFLAGS)

captor.o: captor.cpp captor.h object.o parse_params.o sdl.o ../common/lin_alg.h
	$(CC) -o captor.o -c captor.cpp $(CFLAGS)
	
motor.o: motor.cpp motor.h
	$(CC) -o motor.o -c motor.cpp $(CFLAGS)
	
camera.o: camera.cpp camera.h ../the_cup/the_playground.o
	$(CC) -o camera.o -c camera.cpp $(CFLAGS)

robot.o: object.o motor.o captor.o camera.o robot.cpp robot.h ../pic/simul.h ../common/const.h ../common/comm.h
	$(CC) -o robot.o -c robot.cpp $(CFLAGS)

simulateur.o: simulateur.cpp simulateur.h robot.o parse_params.o keycontrol.o pic/pic_main1.o pic/pic_main2.o the_cup/the_playground.cpp the_cup/the_playground.h
	$(CC) -o simulateur.o -c simulateur.cpp $(CFLAGS)

interface.o: interface.cpp simulateur.o the_cup/the_playground.cpp the_cup/the_playground.h
	$(CC) -o interface.o -c interface.cpp $(CFLAGS)	

clean:
	rm -f *.c~ *.h~ *.cpp~ *.ini~ *.o makefile~ simulateur
