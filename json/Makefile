CC=c++
CPPFLAGS=-Wall -std=c++11
INCLUDES=
COMMON_OBJECTS=json.o

all: $(COMMON_OBJECTS)
	$(CC) $(CPPFLAGS) $(INCLUDES) $(COMMON_OBJECTS) -o json

clean:
	rm -f $(COMMON_OBJECTS) json

%.o : %.cpp
ifeq ($(CC),clang++)
	$(CC) $(CPPFLAGS) -c $<
else
#	check if we need the $(INCLUDES) here
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $<
endif

.PHONY: all clean 
