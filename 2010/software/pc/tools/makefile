include ../../makeopt

all: make_common energyMap

make_common:
	make -C ../../common

energyMap: energyMap/energyMap.cpp dist_transform.o
	$(CC) -o energyMap/energyMap.o -c energyMap/energyMap.cpp $(CFLAGS)
	$(CC) -o energyMap_tool ../../common/bitmap.o ../dist_transform.o energyMap/energyMap.o -f90lib -llapack -lblas $(LDFLAGS)

dist_transform.o: ../dist_transform.cpp ../dist_transform.h
	$(CC) -o ../dist_transform.o -c ../dist_transform.cpp $(CFLAGS)

clean:
	rm -rf *.c~ *.h~ *.cpp~ *.bmp
