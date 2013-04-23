#if !defined (CRAWLER_PROCESS_H)
#define CRAWLER_PROCESS_H

#include <iostream>
#include <vector>
#include <queue>
#include <memory>

class parser;

class crawler_process
{
    public:
    enum worker_status {
        ZOMBIE,         //dead
        IDLE,           //waiting for work
        ACTIVE,         //working
        SLEEP           //blocked (queue)
    };

    struct search_grid_node_s {
        std::string start;
        std::string end;
    };
    
    typedef std::vector<search_grid_node_s> search_grid;
    
    /**
     * contains the post-crawl information about a page
     */
    struct page_node_s {
        std::u16string root_hash;  //16 bit for memory contraint
        std::u16string node_hash;
        unsigned int cash;       //page rank
        std::vector<std::string> meta;
    };
    
    /**
     * url_fifo will only be written to. parent process reads work
     * from fifo to crawler instances
     */
    crawler_process(std::queue<std::string>& url_fifo, search_grid meta_grid, search_grid url_grid);
    virtual ~crawler_process(void);
    
    void crawl(std::string url, struct page_node_s& page_node); //may sleep
    enum worker_status get_status(void);

    private:
    typedef std::vector<std::shared_ptr<parser>> thread_pool;
    
    std::queue<std::string>* fifo;
    enum worker_status status;
    unsigned int meta_threads, url_threads;
    thread_pool meta_parser_threads;
    thread_pool url_parser_threads;
    
};

#endif
