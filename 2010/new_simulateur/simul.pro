CONFIG  *= qt ode debug

QT     *= opengl xml
LIBS   *= -lglut -lode

HEADERS *= xml.h   primitive.h   shape.h   windows.h   object.h   world.h   simulation.h   main.h
SOURCES *= xml.cpp primitive.cpp shape.cpp windows.cpp object.cpp world.cpp simulation.cpp main.cpp

