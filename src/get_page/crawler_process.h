#if !defined (CRAWLER_PROCESS_H)
#define CRAWLER_PROCESS_H

#include <iostream>
#include <vector>

class crawler_process
{
    public:
    /**
     * contains the post-crawl information about a page
     */
    struct page_node_s {
        std::u16string root_hash;  //16 bit for memory contraint
        std::u16string node_hash;
        unsigned int cash;       //page rank
        std::vector<std::string>::keywords;
    };
    
    /*
     * work queue will only be written to. parent process allocates work
     * from queue to crawler instances
     */
    crawler_process(std::queue& work_queue);
    ~crawler_process(void);

    void crawl(std::string url, struct page_node_s& page_node); //may sleep

    private:
    std::queue& work_queue;
    
};

#endif
