#if !defined(CACHE_H)
#define CACHE_H

#include <iostream>
#include <map>
#include <mutex>
#include <chrono>

#include "page_data.hpp"
#include "robots_txt.hpp"

class database;

//total number of robots_txt/page_data_s entries stored in cache
#define PAGE_CACHE_MAX   10
#define ROBOTS_CACHE_MAX 10
//TO DO: number of entries to keep free
#define PAGE_CACHE_RES   0
#define ROBOTS_CACHE_RES 0
//total cache size is thus CACHE_MAX - CACHE_RES

/**
 * data agnostic cache entry
 */
struct cache_entry_s {
    //each entry should only store one
    page_data_s* page;
    robots_txt *robots;

    std::chrono::steady_clock::time_point timestamp;
};

/**
 * used for cache housekeeping
 */
struct cache_ctl_s {
    std::mutex rw_mutex;
    unsigned int fill;
};

enum cache_type {
    PAGE,
    ROBOTS
};

/**
 * data agnostic comparison for sorting cache by most frequently accessed
 */
struct cache_compare {
    bool operator() (struct cache_entry_s * const a, struct cache_entry_s * const b)
    {
        // a - b, larger value is more recent
        std::chrono::duration<double> diff = std::chrono::duration_cast<std::chrono::duration<double>>(a->timestamp, b->timestamp);

        return (diff > 0 ? true:false);
    }
};

typedef std::map<std::string, struct cache_entry_s, cache_compare> cache_map_t;

/**
 * FIXME
 */
class cache
{
    public:
    cache(void);
    ~cache(void);

    /**
     * url is hashed and a page_data_s retrieved.
     * returns true if a page was found in cache
     */
    bool get_page_data(struct page_data_s** page_data, std::string& url);

    /**
     * page is potentially cached, depending on criteria and sent to the db
     * returns true if a page makes it to cache
     */
    bool put_page_data(struct page_data_s* page_data, std::string& url);

    /**
     * same API for robots_txt as page_data_s
     */
    bool get_robots_txt(robots_txt** robots, std::string& url);
    bool put_robots_txt(robots_txt* robots, std::string& url);

    private:
    //tune in the future to specify minimum # of initial buckets
    cache_map_t page_cache;
    cache_map_t robots_cache;
    struct cache_ctl_s page_ctl;
    struct cache_ctl_s robots_ctl;


    //non-threaded
    void prune_cache(cache_type t);
};

#endif
