#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

#include "crawler_process.h"
#include "parser.h"

crawler_process::crawler_process(netio* netio_object, std::queue<std::string>* url_fifo, search_grid meta_grid, search_grid url_grid)
{
    netio_obj = netio_object;
    fifo = url_fifo;
    status = IDLE;

    //allocate parser thread pool per param
    for(search_grid::iterator entry = meta_grid.begin();
        entry != meta_grid.end(); ++entry, ++meta_threads)
    {
        meta_parser_threads.push_back(new parser(entry->start, entry->end));
    }

    for(search_grid::iterator entry = url_grid.begin();
        entry != url_grid.end(); ++entry, ++url_threads)
    {
        url_parser_threads.push_back(new parser(entry->start, entry->end));
    }
}

void delete_object_ptr(parser* const ptr)
{
    delete ptr;
}

crawler_process::~crawler_process(void)
{
    std::for_each(meta_parser_threads.begin(), meta_parser_threads.end(),
        delete_object_ptr);

    std::for_each(url_parser_threads.begin(), url_parser_threads.end(),
        delete_object_ptr);
}

void crawler_process::get_status(enum worker_status &le_status)
{
    le_status = status;
}

void crawler_process::crawl(std::string url, struct page_node_s& page_node)
{
    //analysie url
    //  -- robots.txt information from url root!!
    
    //fetch data

    //run parsers, add compile time option for multithreading here too?
    //  -- each worker_process should already be a thread?
    //  -- originally each worker_process was a fork, with multiple threads
    //      per parser
}
