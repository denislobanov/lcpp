#if !defined(CACHE_H)
#define CACHE_H

#include <iostream>
#include <unordered_map>
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
 */
struct cache_control_s {
    page_data_s* lowest_entry;  //lowest priority/least frequent
    std::mutex rw_mutex;
    unsigned int fill;          //#cache entries
};

typedef std::unordered_map<std::string, struct page_data_s*> cache_map_t;

/**
 * commands for internal housekeeping method
 *  -- later to be threaded, this becomes part of internal ipc
 */
enum cache_task {
    PCACHE_INS,
    PCACHE_RM
};


/**
 * handles both "short term" caching of page_data_s objects in memory
 * and offloading them to the database.
 *
 * class handles all dynamic memory allocation (both for page structures
 * and via robots_txt allocation)
 *
 * NB: only priority cache is currently implemented.
 */
class cache
{
    public:
    cache(database* database_object);
    ~cache(void);

    /**
     * url is hashed and a page_data_s retrieved.
     * if no page_data_s is present, a new one is allocated.
     */
    struct page_data_s* get_page(std::string& url);

    /**
     * page is potentially cached, depending on criteria and sent to the db
     */
    void put_page(std::string& url, struct page_data_s* page_data);

    private:
    //tune in the future to specify minimum # of initial buckets
    cache_map_t priority_cache;
    struct cache_control_s priority_ctl;

    //cache_map_t frequent_cache; //wip / to do
    database* database_obj;

    //non-threaded
    void cache_housekeeping(cache_task task, std::string& url, struct page_data_s* page);
};

#endif
