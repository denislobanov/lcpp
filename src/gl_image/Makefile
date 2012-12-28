CC=clang++
CPPFLAGS=-Wall -std=c++11
INCLUDES=-lGLEW -lGL -lglfw 
OBJECTS=load_file.o gl_image.o shader.o window.o square.o cube.o

all: $(OBJECTS)
	$(CC) $(CPPFLAGS) $(INCLUDES) $(OBJECTS) -o gl_image

clean:
	rm -f $(OBJECTS) gl_image

%.o : %.cpp
ifeq ($(CC),clang++)
	$(CC) $(CPPFLAGS) -c $<
else
#	chack if we need the $(INCLUDES) here
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $<
endif

.PHONY: all clean
