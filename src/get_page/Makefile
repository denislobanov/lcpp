CC=clang++
CPPFLAGS=-Wall -std=c++11
DEPENDENCIES=glibmm-2.4 libxml++-2.6

INCLUDES=$(shell pkg-config --cflags $(DEPENDENCIES))
LDDFLAGS=$(shell curl-config --libs) $(shell pkg-config --libs $(DEPENDENCIES))

COMMON_OBJECTS=netio.o parser.o robots_txt.o database.o cache.o memory_mgr.o queue_client.o crawler_mgr.o
UNIT_TESTS=test_netio test_parser test_crawler test_robots_txt test_cache test_database test_memory_mgr test_html_normalise test_queue_client

all: crawler

clean:
	rm -f *.o
	rm -f test_netio test_parser test_crawler test_robots_txt test_cache test_database test_memory_mgr test_queue_client

crawler: $(COMMON_OBJECTS)
	$(CC) $(LDDFLAGS) -o $@ $(COMMON_OBJECTS)

$(UNIT_TESTS): $(COMMON_OBJECTS)
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $@.cpp
	$(CC) $(LDDFLAGS) -o $@ $@.o $(COMMON_OBJECTS)

%.o : %.cpp
	$(CC) $(CPPFLAGS) $(INCLUDES) -c $<

.PHONY: all clean $(UNIT_TESTS)
