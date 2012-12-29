CC=clang++
CPPFLAGS=-Wall -std=c++11
INCLUDES=-lGLEW -lGL -lglfw 
COMMON_OBJECTS=load_file.o shader.o window.o square.o
CUBE_OBJECTS=cube.o gl_cube.o
SQUARE_OBJECTS=gl_square.o

all: cube

square:	$(COMMON_OBJECTS) $(SQUARE_OBJECTS)
	$(CC) $(CPPFLAGS) $(INCLUDES) $(COMMON_OBJECTS) $(SQUARE_OBJECTS) -o gl_square

cube: $(COMMON_OBJECTS) $(CUBE_OBJECTS)
	$(CC) $(CPPFLAGS) $(INCLUDES) $(COMMON_OBJECTS) $(CUBE_OBJECTS) -o gl_cube

clean:
	rm -f $(COMMON_OBJECTS) $(SQUARE_OBJECTS) $(CUBE_OBJECTS) 
	rm -f gl_square gl_cube

%.o : %.cpp
ifeq ($(CC),clang++)
	$(CC) $(CPPFLAGS) -c $<
else
#	check if we need the $(INCLUDES) here
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $<
endif

.PHONY: all clean cube square
