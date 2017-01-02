CC=clang++
CPPFLAGS=-Wall
INCLUDES=$(shell pkg-config libxml++-2.6 --cflags --libs)
COMMON_OBJECTS=main.o myparser.o

all: $(COMMON_OBJECTS)
	$(CC) $(CPPFLAGS) $(INCLUDES) $(COMMON_OBJECTS) -o main

clean:
	rm -f $(COMMON_OBJECTS) main

%.o : %.cc
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $<

.PHONY: all clean 
