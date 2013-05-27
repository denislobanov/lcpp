CC=clang++
CPPFLAGS=-Wall -std=c++11 $(shell curl-config --cflags)
INCLUDES=$(shell curl-config --libs)
COMMON_OBJECTS=netio.o parser.o robots_txt.o database.o cache.o memory_mgr.o crawler_process.o

all: test_netio test_parser test_crawler test_robots_txt test_cache test_database test_memory_mgr

clean:
	rm -f $(COMMON_OBJECTS)

mrproper: clean
	rm -f *.o
	rm -f test_netio test_parser test_crawler test_robots_txt test_cache test_database test_memory_mgr

test_netio: $(COMMON_OBJECTS) test_netio.o
	$(CC) $(CPPFLAGS) $(INCLUDES) $(COMMON_OBJECTS) test_netio.o -o test_netio

test_parser: $(COMMON_OBJECTS) test_parser.o
	$(CC) $(CPPFLAGS) $(INCLUDES) $(COMMON_OBJECTS) test_parser.o -o test_parser

test_crawler: $(COMMON_OBJECTS) test_crawler.o
	$(CC) $(CPPFLAGS) $(INCLUDES) -I shared_ptr  $(COMMON_OBJECTS) test_crawler.o -o test_crawler

test_robots_txt: $(COMMON_OBJECTS) test_robots_txt.o
	$(CC) $(CPPFLAGS) $(INCLUDES) -I shared_ptr  $(COMMON_OBJECTS) test_robots_txt.o -o test_robots_txt

test_cache: $(COMMON_OBJECTS) test_cache.o
	$(CC) $(CPPFLAGS) $(INCLUDES) -I shared_ptr  $(COMMON_OBJECTS) test_cache.o -o test_cache

test_database: $(COMMON_OBJECTS) test_database.o
	$(CC) $(CPPFLAGS) $(INCLUDES) -I shared_ptr  $(COMMON_OBJECTS) test_database.o -o test_database

test_memory_mgr: $(COMMON_OBJECTS) test_memory_mgr.o
	$(CC) $(CPPFLAGS) $(INCLUDES) -I shared_ptr  $(COMMON_OBJECTS) test_memory_mgr.o -o test_memory_mgr

%.o : %.cpp
ifeq ($(CC),clang++)
	$(CC) $(CPPFLAGS) -c $<
else
#	check if we need the $(INCLUDES) here
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $<
endif

.PHONY: all clean
