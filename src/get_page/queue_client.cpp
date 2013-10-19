#include <iostream>
#include <mutex>
#include <queue>

#include "queue_client.hpp"

//Local defines
#define DEBUG 1

#if defined(DEBUG)
    #define dbg std::cout<<__FILE__<<"("<<__LINE__<<"): "
    #if DEBUG > 1
        #define dbg_1 std::cout<<__FILE__<<"("<<__LINE__<<"): "
    #else
        #define dbg_1 0 && std::cout
    #endif
#else
    #define dbg 0 && std::cout
    #define dbg_1 0 && std::cout
#endif

queue_client::queue_client(qc_config& config)
{
    //todo
}

queue_client::~queue_client(void)
{
    //?
}

void queue_client::send(struct queue_node_s& data)
{
    queue_lock.lock();
    dev_queue.push(data);
    queue_lock.unlock();

    dbg<<"pushed data to queue [credit: "<<data.credit<<" url: "<<data.url<<"]\n";
}

struct queue_node_s queue_client::fetch(void)
{
    struct queue_node_s data = {0};

    queue_lock.lock();
    if(!dev_queue.empty()) {
        dbg<<"queue not empty..\n";
        data = dev_queue.front();
        dev_queue.pop();
    } else {
        exit(-4);
    }
    queue_lock.unlock();

    dbg<<"returning data from queue [credit: "<<data.credit<<" url: "<<data.url<<"]\n";
    return data;
}

void queue_client::tax_in(unsigned int tax)
{
    return;
}
