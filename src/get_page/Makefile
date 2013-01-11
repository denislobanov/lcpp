CC=clang++
CPPFLAGS=-Wall -std=c++11 $(shell curl-config --cflags)
INCLUDES=$(shell curl-config --libs)
COMMON_OBJECTS=netio.o parser.o

all: test_parser

get_page: $(COMMON_OBJECTS)
	$(CC) $(CPPFLAGS) $(INCLUDES) $(COMMON_OBJECTS) -o get_page

clean:
	rm -f $(COMMON_OBJECTS)

test_netio: $(COMMON_OBJECTS) test_netio.o
	$(CC) $(CPPFLAGS) $(INCLUDES) $(COMMON_OBJECTS) test_netio.o -o test_netio

test_parser: $(COMMON_OBJECTS) test_parser.o
	$(CC) $(CPPFLAGS) $(INCLUDES) $(COMMON_OBJECTS) test_parser.o -o test_parser

%.o : %.cpp
ifeq ($(CC),clang++)
	$(CC) $(CPPFLAGS) -c $<
else
#	check if we need the $(INCLUDES) here
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $<
endif

.PHONY: all clean 
