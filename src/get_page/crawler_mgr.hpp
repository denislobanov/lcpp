#if !defined (CRAWLER_PROCESS_H)
#define CRAWLER_PROCESS_H

#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <cstdint>

#include "page_data.hpp"
#include "memory_mgr.hpp"
#include "netio.hpp"
#include "parser.hpp"
#include "queue_client.hpp"

class parser;
class netio;

/**
 * Global, part of objects interface
 */
#define ROBOTS_REFRESH  60*5   //seconds

/**
 * status of crawler_worker
 * FIXME: this class should become the worker, move definition to manager
 */
enum worker_status {
    ZOMBIE,         //dead
    IDLE,           //waiting for work
    ACTIVE,         //working
    SLEEP           //blocked (queue)
};

class crawler_mgr
{
    public:
    /**
     * url_fifo will only be written to. parent process reads work
     * from fifo to crawler instances
     */
    crawler_mgr(netio* nio, queue_client* qc, memory_mgr* mm, std::vector<struct parse_param_s>& parse_param);
    ~crawler_mgr(void);

    /**
     * crawl #i items from queue. development implementation.
     *
     * urls found during the crawl are automattically appended to queue specified on object creation
     */
    void loop(int i); //may sleep

    /**
     * returns status of crawler process, value will be one of worker_status enum
     */
    void get_status(enum worker_status &crawler_status);

    private:
    enum worker_status status;
    parser* single_parser;
    std::string data;

    //objects passed to construtor
    netio* netio_obj;
    queue_client* queue_obj;
    memory_mgr* mem_mgr;

    size_t root_domain(std::string& url);
};

#endif
