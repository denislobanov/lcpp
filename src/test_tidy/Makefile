CC=gcc
CPPFLAGS=-Wall $(shell curl-config --cflags)
INCLUDES=$(shell curl-config --libs) -ltidy
COMMON_OBJECTS=tidy.o

all: $(COMMON_OBJECTS)
	$(CC) $(CPPFLAGS) $(INCLUDES) $(COMMON_OBJECTS) -o test_tidy

clean:
	rm -f $(COMMON_OBJECTS) test_tidy

%.o : %.gcc
ifeq ($(CC),clang)
	$(CC) $(CPPFLAGS) -c $<
else
#	check if we need the $(INCLUDES) here
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $<
endif

.PHONY: all clean 
