CC=clang++
CPPFLAGS=-Wall -std=c++11 -v

INCLUDES=-I/home/vision/dev/xdrc/build/boost/include
LDDFLAGS=-L/home/vision/dev/xdrc/build/boost/lib/
LIBRARIES=-lboost_system -lpthread

COMMON_OBJECTS=sync_ipc.o

all: $(COMMON_OBJECTS)
	$(CC) $(CPPFLAGS) $(INCLUDES) $(COMMON_OBJECTS) -o sync_ipc $(LDDFLAGS) $(LIBRARIES) 
clean:
	rm -f $(UNIT_TESTS) *.o

%.o : %.cpp
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $<

.PHONY: all clean $(UNIT_TESTS)
