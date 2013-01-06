CC=clang++
CPPFLAGS=-Wall -std=c++11 $(shell curl-config --cflags)
INCLUDES=$(shell curl-config --libs)
COMMON_OBJECTS=netio.o test_netio.o

all: get_page

get_page: $(COMMON_OBJECTS)
	$(CC) $(CPPFLAGS) $(INCLUDES) $(COMMON_OBJECTS) -o get_page

clean:
	rm -f $(COMMON_OBJECTS)

%.o : %.cpp
ifeq ($(CC),clang++)
	$(CC) $(CPPFLAGS) -c $<
else
#	check if we need the $(INCLUDES) here
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $<
endif

.PHONY: all clean 
