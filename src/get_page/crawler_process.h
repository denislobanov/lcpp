#if !defined (CRAWLER_PROCESS_H)
#define CRAWLER_PROCESS_H

#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <cstdint>

#include "page_data.hpp"

class parser;
class netio;

/**
 * Global, part of objects interface
 */

/**
 * status of crawler_process
 */
enum worker_status {
    ZOMBIE,         //dead
    IDLE,           //waiting for work
    ACTIVE,         //working
    SLEEP           //blocked (queue)
};

/**
 * a search grid is composed of two elements, commonly representing a
 * html tag open and close.
 *
 * these parameters control the text areas that will be parsed for keywords
 * or other urls.
 */
struct search_grid_node_s {
    std::string start;
    std::string end;
};

/**
 * a search grid is a vectore of one or more nodes
 */
typedef std::vector<search_grid_node_s> search_grid;

class crawler_process
{
    public:    
    /**
     * url_fifo will only be written to. parent process reads work
     * from fifo to crawler instances
     */
    crawler_process(netio* netio_object, std::queue<std::string>* url_fifo, search_grid meta_grid, search_grid url_grid);
    ~crawler_process(void);

    /**
     * retrieves data pointed to by url, performs crawl based on meta_grid
     * and url_grid specified on objects creation, returning a page_node
     * describing the data.
     *
     * urls found during the crawl are automattically appended to queue specified on object creation
     */
    void crawl(std::string url, struct page_node_s& page_node); //may sleep

    /**
     * returns status of crawler process, value will be one of worker_status enum
     */
    void get_status(enum worker_status &le_status);

    private:
    typedef std::vector<parser*> thread_pool;

    thread_pool meta_parser_threads;
    thread_pool url_parser_threads;
    enum worker_status status;
    std::string web_data;
    std::hash<std::string> url_hash;
    
    std::queue<std::string>* fifo;
    netio* netio_obj;
};

#endif
