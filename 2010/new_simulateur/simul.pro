CONFIG  *= qt ode

QT     *= opengl xml
LIBS   *= -lglut -lode

HEADERS *= xml.h   shape.h   windows.h   object.h   world.h   simulation.h   main.h
SOURCES *= xml.cpp shape.cpp windows.cpp object.cpp world.cpp simulation.cpp main.cpp
