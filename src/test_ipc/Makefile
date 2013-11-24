CC=clang++
CPPFLAGS=-Wall -std=c++11

INCLUDES=-I/usr/include/boost
LDDFLAGS=

COMMON_OBJECTS=ipc_test.o

all: $(COMMON_OBJECTS)
	$(CC) $(CPPFLAGS) $(INCLUDES) $(COMMON_OBJECTS) -o ipc_test

clean:
	rm -f $(UNIT_TESTS) *.o

%.o : %.cpp
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $<

.PHONY: all clean
