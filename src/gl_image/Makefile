CC=g++
CPPFLAGS=-Wall -std=c++11
INCLUDES=-lGLEW -lGL -lglfw 
OBJECTS=gl_image.o load_file.o shader.o window.o square.o

all: $(OBJECTS)
	$(CC) $(CPPFLAGS) $(INCLUDES) $(OBJECTS) -o gl_image

%.o : %.c
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $<
