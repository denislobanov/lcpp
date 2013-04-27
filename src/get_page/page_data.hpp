#if !defined (PAGE_DATA_H)
#define PAGE_DATA_H

#include <vector>
#include <ctime>

struct page_node_s {
    unsigned int cash;       //page rank
    std::vector<std::string> meta;  //keywords associated with page

    //for root pages only (top level domains)
    time_t last_visit;
    time_t timeout;
};


#endif
