include ../makeopt

all: make_common main.o
	$(CC) -o ../client ../common/console.o ../common/comm.o ../common/bitmap.o usb.o cinematik.o webcamAPI.o path_planner.o picAPI.o anticol.o path_tracker.o strategie.o sdl.o visualizer.o pc_main.o main.o webcam_processing.o webcam_utils.o webcam_common.o webcam.o -f90lib -llapack -lblas $(LDFLAGS)

make_common:
	make -C ../common
	
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

path_tracker.o: path_tracker.cpp path_tracker.h path_planner.o ../common/simul.h types.h
	$(CC) -o path_tracker.o -c path_tracker.cpp $(CFLAGS)

strategie.o: strategie.cpp strategie.h path_tracker.o picAPI.o ../common/simul.h types.h ../common/const.h
	$(CC) -o strategie.o -c strategie.cpp $(CFLAGS)
		
sdl.o: sdl.cpp sdl.h
	$(CC) -o sdl.o -c sdl.cpp $(CFLAGS)
	
visualizer.o: visualizer.cpp visualizer.h sdl.o
	$(CC) -o visualizer.o -c visualizer.cpp $(CFLAGS)
		
pc_main.o: pc_main.cpp pc_main.h usb.o anticol.o strategie.o ../common/simul.h types.h sdl.o visualizer.o
	$(CC) -o pc_main.o -c pc_main.cpp $(CFLAGS)

main.o: main.cpp main.h pc_main.o ../common/simul.h types.h webcamAPI.o
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
	rm -f *.c~ simul/*.c~ *.h~ *.hpp *.cpp~ *.ini~ *.o makefile~ simulateur
