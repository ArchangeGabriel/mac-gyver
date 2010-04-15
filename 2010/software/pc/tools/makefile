include ../../makeopt

all: make_common energyMap

make_common:
	make -C ../../common

energyMap: energyMap/energyMap.cpp path_planner.o
	$(CC) -o energyMap/energyMap.o -c energyMap/energyMap.cpp $(CFLAGS)
	$(CC) -o energyMap_tool ../../common/bitmap.o ../path_planner.o energyMap/energyMap.o -f90lib -llapack -lblas $(LDFLAGS)

path_planner.o: ../path_planner.cpp ../path_planner.h
	$(CC) -o ../path_planner.o -c ../path_planner.cpp $(CFLAGS)

clean:
	rm -rf *.c~ *.h~ *.cpp~ *.bmp
