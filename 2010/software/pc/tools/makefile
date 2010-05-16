include ../../makeopt

all: make_common energyMap view_wc_tool

make_common:
	make -C ../../common
	make -C ../

energyMap: energyMap/energyMap.cpp path_planner.o
	$(CC) -o energyMap/energyMap.o -c energyMap/energyMap.cpp $(CFLAGS)
	$(CC) -o energyMap_tool ../../common/bitmap.o ../path_planner.o energyMap/energyMap.o -f90lib -llapack -lblas $(LDFLAGS)

view_wc_tool: view_wc/view_wc.cpp
	$(CC) -o view_wc/view_wc.o -c view_wc/view_wc.cpp $(CFLAGS)
	$(CC) -o view_wc_tool ../../common/bitmap.o ../sdl.o ../webcam.o ../webcam_common.o ../webcam_processing.o view_wc/view_wc.o $(LDFLAGS)

find_lines_tool: find_lines/find_lines.cpp
	$(CC) -o find_lines/find_lines.o -c find_lines/find_lines.cpp $(CFLAGS)
	$(CC) -o find_lines_tool ../../common/bitmap.o ../sdl.o ../webcam.o ../webcam_common.o ../webcam_filter.o ../webcam_utils.o ../webcam_processing.o find_lines/find_lines.o $(LDFLAGS)

path_planner.o: ../path_planner.cpp ../path_planner.h
	$(CC) -o ../path_planner.o -c ../path_planner.cpp $(CFLAGS)	

clean:
	rm -rf *.c~ *.h~ *.cpp~ *.bmp energyMap_tool view_wc_tool find_lines_tool
