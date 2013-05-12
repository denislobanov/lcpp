#if !defined(CACHE_H)
#define CACHE_H

#include <iostream>
#include <ctime>
#include <mutex>

#include "page_data.hpp"

class database;

//point at which items will need to be removed
#define PC_UPPER_WATERMARK  20
//for delayed freeing (housekeeping /thread/ implementation)
#define PC_MAX_FILL         0//unused!

//frequency cache control
#define FC_UPPER_WATERMARK  10
#define FC_MAX_FILL         0//unused!

/**
 * part of the cache control mechanism
 *  delta is the page rank or last access time in seconds since the unix epoch
 *  url is the page url use to perform key lookup in cache
 */
struct cache_control_s {
    unsigned int delta; //lowest pri/last cache time
    std::string url;
};

typedef std::unordered_map<std::string, struct page_data_s*> cache_map_t;

/**
 * commands for internal housekeeping method
 *  -- later to be threaded, this becomes part of internal ipc
 */
enum cache_task {
    PRUNE_PC,
    EVAL_PC,
    PRUNE_FC
};


/**
 * handles both "short term" caching of page_data_s objects in memory
 * and offloading them to the database.
 *
 * class handles all dynamic memory allocation (both for page structures
 * and via robots_txt allocation)
 */
class cache
{
    public:
    cache(netio* netio_object, database* database_object);
    ~cache(void);

    /**
     * url is hashed and a page_data_s retrieved.
     * if no page_data_s is present, a new one is allocated.
     */
    struct page_data_s* get_page(std::string& url);

    /**
     * page is potentially cached, depending on criteria and sent to the db
     */
    void put_page(struct page_data_s* page, std::string& url);

    private:
    //tune in the future to specify minimum # of initial buckets
    cache_map_t priority_cache;
    cache_map_t frequent_cache;
    
    //cache control var
    std::mutex priority_cache_mutex;
    struct cache_control_s priority_ctl;
    unsigned int priority_cache_fill;
    
    std::mutex frequent_cache_mutex;
    std::vector<struct cache_control_s> frequent_ctl;
    unsigned int frequent_cache_fill;

    //to pass to robots_txt classes.
    netio* netio_obj;
    database* database_obj;

    void cache_housekeeping_nt(cache_task task, std::string& url, struct page_data_s* page);
    
};

#endif
