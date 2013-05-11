#if !defined(CACHE_H)
#define CACHE_H

#include <iostream>

#include "page_data.hpp"

class database;

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
    std::unordered_map<
    std::vector<struct page_data_s*> access_time_cache;
    std::vector<struct page_data_s*> priority_cache;

    //cache control var

    //to pass to robots_txt classes.
    netio* netio_obj;
    database* database_obj;
    
};

#endif
