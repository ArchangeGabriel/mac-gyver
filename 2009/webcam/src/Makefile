CC=g++
CFLAGS=-W -Wall -O2 -g
LDFLAGS=-lIL -lILU
SRC= $(wildcard *.cpp)
OBJ= $(SRC:.cpp=.o)

all: test_webcam

test_webcam: test_webcam.o webcam.o processing.o utils.o common.o filter.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(OBJ)


