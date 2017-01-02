CC=clang++
CPPFLAGS=-Wall -std=c++11 $(shell curl-config --cflags)
INCLUDES=$(shell curl-config --libs)
COMMON_OBJECTS=allocation_test.o

all: $(COMMON_OBJECTS)
	$(CC) $(CPPFLAGS) $(INCLUDES) $(COMMON_OBJECTS) -o allocation_test

clean:
	rm -f $(COMMON_OBJECTS) allocation_test

%.o : %.cpp
ifeq ($(CC),clang++)
	$(CC) $(CPPFLAGS) -c $<
else
#	check if we need the $(INCLUDES) here
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $<
endif

.PHONY: all clean 
