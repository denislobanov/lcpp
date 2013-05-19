#include <iostream>
#include <unordered_map>
#include <mutex>

#include "cache.hpp"
#include "database.hpp"
#include "page_data.hpp"

//Local defines
#if defined(DEBUG)
    #define dbg std::cout<<__FILE__<<"("<<__LINE__<<"): "
    #if DEBUG > 1
        #define dbg_1 std::cout<<__FILE__<<"("<<__LINE__<<"): "
    #else
        #define dbg_1 0 && std::cout
    #endif
#else
    #define dbg 0 && std::cout
    #define dbg_1 0 && std::cout
#endif

cache::cache(database* database_object)
{
    database_obj = database_object;
}

cache::~cache(void)
{
    //delete cache items
    for(cache_map_t::iterator it = priority_cache.begin(); it != priority_cache.end(); ++it)
        delete it->second;
}

//non threading version. assumes locks have already been aquired.
void cache::cache_housekeeping(cache_task task, std::string& url, struct page_data_s* page)
{
    switch(task) {
    case PRUNE_PCACHE:
        //remove from cache
        if(page->rank < priority_ctl.lowest_entry->rank) {
            priority_cache.erase(url);
            --priority_ctl.fill;
        }
        break;
    case EVAL_PCACHE:
        //go through cache, find lowest rank
        break;
    case PRUNE_FCACHE:
        //tbd
        dbg<<"PRUNE_FCACHE - frequent access caching not supported"<<std::endl;
        break;
    default:
        std::cerr<<"cache::cache_housekeeping_nt given unknown task"<<std::endl;
    }
}

struct page_data_s* cache::get_page(std::string& url)
{
    struct page_data_s* page_data = nullptr;

    //both getting and putting pages in cache have to be atomic
    priority_ctl.rw_mutex.lock();
    
    //check priority cache first
    cache_map_t::iterator pri_node = priority_cache.find(url);
    if(pri_node != priority_cache.end()) {
        page_data = pri_node->second;

        //we have a page, lock its in-use mutex
        page_data->access_lock.lock();
    }

    //cache has to be unlocked before calls to the database to prevent slowdown
    priority_ctl.rw_mutex.unlock();

    //not in cache, try db
    if(page_data == nullptr) {
        page_data = database_obj->get_page(url); //has its own locking

        if(page_data == nullptr) //allocate new page. we should do this from a pool
            page_data = new struct page_data_s;

        page_data->access_lock.lock();
    }

    return page_data;
}

//page access lock assumed
void cache::put_page(std::string& url, struct page_data_s* page_data)
{
    std::pair<std::string, struct page_data_s*> page (url, page_data);
    
    priority_ctl.rw_mutex.lock();
    page_data->access_lock.unlock();

    if(page_data->rank > priority_ctl.lowest_entry->rank) {
        if(priority_ctl.fill >= PC_UPPER_WATERMARK) {
            //will need to remove existing page
            priority_cache.erase(priority_ctl.lowest_entry->url);
            delete(&priority_ctl.lowest_entry);

            //update cache_ctl
            cache_housekeeping(PRUNE_PCACHE, url, page_data);
        }

        priority_cache.insert(page);
    }
    priority_ctl.rw_mutex.unlock();

    //send to db
    database_obj->put_page(url, page_data);
}

