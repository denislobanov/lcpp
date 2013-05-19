#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <functional>

#include "crawler_process.hpp"
#include "parser.hpp"
#include "netio.hpp"
#include "robots_txt.hpp"

//should not be needed with new database class?
#include "page_data.hpp"

crawler_process::crawler_process(netio* netio_object, std::queue<std::string>* url_fifo, search_grid meta_grid, search_grid url_grid)
{
    netio_obj = netio_object;
    fifo = url_fifo;
    status = IDLE;

    //allocate parser thread pool per param
    for(search_grid::iterator entry = meta_grid.begin();
        entry != meta_grid.end(); ++entry)
    {
        meta_parser_threads.push_back(new parser(entry->start, entry->end));
    }

    for(search_grid::iterator entry = url_grid.begin();
        entry != url_grid.end(); ++entry)
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

size_t crawler_process::root_domain(std::string& url)
{
    //  0123456
    // "http://" next "/" is at the end of the root url
    return url.find_first_of("/", 7);
}

void crawler_process::crawl(std::string url, struct page_data_s& page_node)
{

//needs refactoring once cache and database are define
#if 0
    //analysie url
    //  -- robots.txt information from url root!!
    //~ page_node.root_hash = 0;    //WIP
    //~ page_node.node_hash = (uint16_t)url_hash(url);
    page_node.rank = 0; //WIP

    //fetch data
    netio_obj->fetch(&web_data, url);
    std::cout<<"size of data retrieved: "<<web_data.size()<<std::endl;

    //run parsers, add compile time option for multithreading here too?
    //  -- each worker_process should already be a thread?
    //  -- originally each worker_process was a fork, with multiple threads
    //      per parser
    for(thread_pool::iterator thread = meta_parser_threads.begin();
        thread != meta_parser_threads.end(); ++thread)
    {
        unsigned int count;

        (*thread)->parse(web_data);
        count = (*thread)->extract(web_data, page_node.meta);
        std::cout<<"extrated "<<count<<" keywords.."<<std::endl;
    }

    for(thread_pool::iterator thread = url_parser_threads.begin();
        thread != url_parser_threads.end(); ++thread)
    {

        (*thread)->parse(web_data);
        std::cout<<"parsed for urls. not doing extration cause programming is hard"<<std::endl;
    }
#endif
}
