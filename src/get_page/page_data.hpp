#if !defined (PAGE_DATA_H)
#define PAGE_DATA_H

#include <vector>
#include <time.h>
#include <mutex>

#include "robots_txt.hpp"

/**
 * describes an entry to pass to caching/db mechanism
 */
struct page_data_s {
    std::string url;
    unsigned int rank;
    std::vector<std::string> meta;  //keywords associated with page
    std::string description;        //short blob about page

    //for root pages only (top level domains)
    time_t last_visit;
    time_t crawl_delay;
    robots_txt* robots;

    //book keeping
    std::mutex access_lock; //only one thread may access at a time, managed by cache class
};

/**
 * describes an entry in the crawl (IPC) queue
 */
struct queue_node_s {
    unsigned int credit;    //cash given to link from referring page
    std::string url;
};

#endif
