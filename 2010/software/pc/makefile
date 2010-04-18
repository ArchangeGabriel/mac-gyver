include ../makeopt

all: make_common make_picAPI main.o
	$(CC) -o ../client ../common/console.o ../common/comm.o ../common/bitmap.o common.o usb.o picAPI/pic_interface.o picAPI/usbdeviceinout.o picAPI/usbdevicemoteurs.o cinematik.o webcamAPI.o path_planner.o picAPI.o anticol.o path_tracker.o strategie.o sdl.o visualizer.o main.o webcam_processing.o webcam_utils.o webcam_common.o webcam.o -f90lib -llapack -lblas $(LDFLAGS)

make_common:
	make -C ../common
	
make_picAPI:
	make -C picAPI	
	
common.o: common.cpp common.h
	$(CC) -o common.o -c common.cpp $(CFLAGS)
	
usb.o: usb.cpp usb.h simul/usb.cpp ../common/comm.h ../common/simul.h
	$(CC) -o usb.o -c usb.cpp $(CFLAGS)

cinematik.o: cinematik.cpp cinematik.h ../common/const.h
	$(CC) -o cinematik.o -c cinematik.cpp $(CFLAGS)

webcamAPI.o: webcamAPI.cpp webcamAPI.h ../common/simul.h webcam.o webcam_processing.o webcam_utils.o
	$(CC) -o webcamAPI.o -c webcamAPI.cpp $(CFLAGS)
	
path_planner.o: path_planner.cpp path_planner.h
	$(CC) -o path_planner.o -c path_planner.cpp $(CFLAGS)
		
picAPI.o: picAPI.cpp picAPI.h usb.o cinematik.o ../common/simul.h
	$(CC) -o picAPI.o -c picAPI.cpp $(CFLAGS)

anticol.o: anticol.cpp anticol.h
	$(CC) -o anticol.o -c anticol.cpp $(CFLAGS)	

path_tracker.o: path_tracker.cpp path_tracker.h path_planner.o ../common/simul.h common.o cinematik.o
	$(CC) -o path_tracker.o -c path_tracker.cpp $(CFLAGS)

strategie.o: strategie.cpp strategie.h path_tracker.o picAPI.o ../common/simul.h common.o ../common/const.h
	$(CC) -o strategie.o -c strategie.cpp $(CFLAGS)
		
sdl.o: sdl.cpp sdl.h
	$(CC) -o sdl.o -c sdl.cpp $(CFLAGS)
	
visualizer.o: visualizer.cpp visualizer.h sdl.o
	$(CC) -o visualizer.o -c visualizer.cpp $(CFLAGS)
		
main.o: main.cpp ../common/simul.h common.o picAPI.o strategie.o anticol.o visualizer.o webcamAPI.o
	$(CC) -o main.o -c main.cpp $(CFLAGS)

webcam.o: webcam.cpp webcam.hpp webcam_common.o
	$(CC) -o webcam.o -c webcam.cpp $(CFLAGS)

webcam_processing.o: webcam_processing.cpp webcam_processing.hpp
	$(CC) -o webcam_processing.o -c webcam_processing.cpp $(CFLAGS)

webcam_utils.o: webcam_utils.cpp webcam_utils.hpp
	$(CC) -o webcam_utils.o -c webcam_utils.cpp $(CFLAGS)

webcam_filter.o: webcam_filter.cpp webcam_filter.hpp
	$(CC) -o webcam_filter.o -c webcam_filter.cpp $(CFLAGS)
    
webcam_common.o: webcam_common.cpp webcam_common.hpp
	$(CC) -o webcam_common.o -c webcam_common.cpp $(CFLAGS)

clean:
	rm -rf *.c~ simul/*.c~ *.h~ *.hpp~ *.cpp~ *.ini~ *.o makefile~ simulateur
	make -s -C picAPI clean
