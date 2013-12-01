CC=clang++
CPPFLAGS=-Wall -std=c++11

INCLUDES=-I/home/vision/dev/xdrc/build/boost/include
LDDFLAGS=-L/home/vision/dev/xdrc/build/boost/lib/
LIBRARIES=-lboost_system -lpthread

TESTS=sync_ipc

all: $(TESTS)

clean:
	rm -f $(TESTS) *.o

$(TESTS):
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $@.cpp
	$(CC) $(CPPFLAGS) $(INCLUDES) $@.o -o $@ $(LDDFLAGS) $(LIBRARIES)

.PHONY: all clean $(TESTS)
