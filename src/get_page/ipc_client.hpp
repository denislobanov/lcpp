#if !defined (IPC_CLIENT_H)
#define IPC_CLIENT_H

#include <iostream>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <system_error>

#include "ipc_common.hpp"
#include "page_data.hpp"

class ipc_client
{
    public:
    ipc_client(void);
    ~ipc_client(void);

    //send discovered/crawled pages to master, will throw system_error if IPC failed
    void send_item(struct queue_node_s& data) throw(std::system_error);

    //sends current worker status to master, will throw system_error if IPC failed
    void send_status(worker_status status) throw(std::system_error);

    //sends capabilities to master, will throw system_error if IPC failed
    void send_capabilities(struct capabilities& cap) throw(std::system_error);

    //send configuration to master, will throw system_error if IPC failed
    void send_config(struct worker_config& config) throw(std::system_error);

    //gets a new page to crawl from master, will throw underrun_exception
    //if failed to get any data or system_error if IPC failed
    struct queue_node_s get_item(void) throw(std::underflow_error);

    //get configuration from master, will throw system_error if IPC failed
    struct worker_config get_config(void) throw(std::system_error);

    //FIXME: make this event driven
    //blocking get of instructions form master
    worker_intruction get_intstruction(void) throw(std::system_error);

    private:
    //no actual IPC for current development stage
    std::queue<struct queue_node_s> dev_queue;
    std::mutex queue_lock;
};

#endif
