include ../makeopt

all: make_common main.o
	$(CC) -o ../client ../common/console.o ../common/comm.o usb.o cinematik.o webcamAPI.o dist_transform.o picAPI.o anticol.o path_planning.o strategie.o sdl.o visualizer.o pc_main.o main.o processing.o utils.o common.o webcam.o $(LDFLAGS)

make_common:
	make -C ../common
	
usb.o: usb.cpp usb.h simul/usb.cpp ../common/comm.h ../common/simul.h
	$(CC) -o usb.o -c usb.cpp $(CFLAGS)

cinematik.o: cinematik.cpp cinematik.h ../common/const.h
	$(CC) -o cinematik.o -c cinematik.cpp $(CFLAGS)

webcamAPI.o: webcamAPI.cpp webcamAPI.h ../common/simul.h webcam.o processing.o utils.o
	$(CC) -o webcamAPI.o -c webcamAPI.cpp $(CFLAGS)
	
dist_transform.o: dist_transform.cpp dist_transform.h
	$(CC) -o dist_transform.o -c dist_transform.cpp $(CFLAGS)
		
picAPI.o: picAPI.cpp picAPI.h usb.o cinematik.o ../common/simul.h
	$(CC) -o picAPI.o -c picAPI.cpp $(CFLAGS)

anticol.o: anticol.cpp anticol.h
	$(CC) -o anticol.o -c anticol.cpp $(CFLAGS)	

path_planning.o: path_planning.cpp path_planning.h dist_transform.o ../common/simul.h types.h
	$(CC) -o path_planning.o -c path_planning.cpp $(CFLAGS)

strategie.o: strategie.cpp strategie.h path_planning.o picAPI.o ../common/simul.h types.h ../common/const.h
	$(CC) -o strategie.o -c strategie.cpp $(CFLAGS)
		
sdl.o: sdl.cpp sdl.h
	$(CC) -o sdl.o -c sdl.cpp $(CFLAGS)
	
visualizer.o: visualizer.cpp visualizer.h sdl.o
	$(CC) -o visualizer.o -c visualizer.cpp $(CFLAGS)
		
pc_main.o: pc_main.cpp pc_main.h usb.o anticol.o strategie.o ../common/simul.h types.h sdl.o visualizer.o
	$(CC) -o pc_main.o -c pc_main.cpp $(CFLAGS)

main.o: main.cpp main.h pc_main.o ../common/simul.h types.h webcamAPI.o
	$(CC) -o main.o -c main.cpp $(CFLAGS)

webcam.o: webcam.cpp webcam.hpp common.o
	$(CC) -o webcam.o -c webcam.cpp $(CFLAGS)

processing.o: processing.cpp processing.hpp
	$(CC) -o processing.o -c processing.cpp $(CFLAGS)

utils.o: utils.cpp utils.hpp
	$(CC) -o utils.o -c utils.cpp $(CFLAGS)

filter.o: filter.cpp filter.hpp
	$(CC) -o filter.o -c filter.cpp $(CFLAGS)
    
common.o: common.cpp common.hpp
	$(CC) -o common.o -c common.cpp $(CFLAGS)

clean:
	rm -f *.c~ simul/*.c~ *.h~ *.cpp~ *.ini~ *.o makefile~ simulateur
