CC=g++
CPPFLAGS=-Wall -std=c++11
INCLUDES=-lGLEW -lGL -lglfw 
OBJECTS=gl_image.o load_file.o shader.o window.o square.o

all: $(OBJECTS)
	$(CC) $(CPPFLAGS) $(INCLUDES) $(OBJECTS) -o gl_image

#square.o: square.cpp
#	$(CC) $(CPPFLAGS) $(INCLUDES) load_file.o -c -o square.o square.cpp

%.o : %.c
	$(CC) $(CPPFLAGS) $(INCLUDES) load_file.o -c $<
