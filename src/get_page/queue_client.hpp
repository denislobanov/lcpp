#if !defined (QUEUE_CLIENT_H)
#define QUEUE_CLIENT_H

#include <iostream>
#include <mutex>
#include <queue>

#include "page_data.hpp"

//placeholder for now. proper implementation will include networks IPC
typedef int qc_config;

class queue_client
{
    public:
    queue_client(qc_config& config);
    ~queue_client(void);

    void send(struct queue_node_s& data);
    struct queue_node_s fetch(void);

    void tax_in(unsigned int tax);

    private:
    //local queue for development implementation only.
    std::queue<struct queue_node_s> dev_queue;
    std::mutex queue_lock;
};

#endif
