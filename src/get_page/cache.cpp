#include <iostream>
#include <unordered_map>
#include <mutex>

#include "cache.hpp"
#include "database.hpp"
#include "page_data.hpp"

#define DEBUG 1

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
    priority_ctl.lowest_entry = nullptr;
    priority_ctl.fill = 0;
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
    case PCACHE_INS:
        if(priority_ctl.lowest_entry == nullptr) {
            priority_ctl.lowest_entry = page;
            dbg<<"first lowest entry since time"<<std::endl;
        } else {
            if (page->rank < priority_ctl.lowest_entry->rank) {
                dbg<<"page rank "<<page->rank<<" is the new lowest entry"<<std::endl;
                priority_ctl.lowest_entry = page;
            }
        }

        ++priority_ctl.fill;
        dbg<<"priority_ctl.fill "<<priority_ctl.fill<<std::endl;
        break;

    case PCACHE_RM:
        //remove existing lowest entry from cache
        priority_cache.erase(priority_ctl.lowest_entry->url);
        delete priority_ctl.lowest_entry;

        //find new lowest entry
        for(unsigned int i=0; i<priority_cache.bucket_count(); ++i) {
            dbg<<"bucket "<<i<<" ";

            for(auto le_it = priority_cache.begin(i); le_it != priority_cache.end(i); ++le_it) {
                std::cout<<"rank "<<le_it->second->rank<<" ";

                if(priority_ctl.lowest_entry == nullptr) {
                    std::cout<<"first lowest entry";
                    priority_ctl.lowest_entry = le_it->second;
                }else if(le_it->second->rank < priority_ctl.lowest_entry->rank) {
                    std::cout<<"replaced lowest entry";
                    priority_ctl.lowest_entry = le_it->second;
                }
            }
            dbg<<std::endl;
        }
        break;

    default:
        std::cerr<<"cache::cache_housekeeping_nt given unknown task"<<std::endl;
        break;
    }
}

struct page_data_s* cache::get_page(std::string& url)
{
    struct page_data_s* page_data = nullptr;

    //both getting and putting pages in cache have to be atomic
    priority_ctl.rw_mutex.lock();

    cache_map_t::iterator pri_node = priority_cache.find(url);
    if(pri_node != priority_cache.end()) {
        page_data = pri_node->second;

        //we have a page, lock its in-use mutex
        page_data->access_lock.lock();
        dbg<<"got page"<<std::endl;
    }

    //cache has to be unlocked before calls to the database to prevent slowdown
    priority_ctl.rw_mutex.unlock();

    //not in cache, try db then allocate
    if(page_data == nullptr) {
        dbg<<"page_data == nullptr"<<std::endl;
        page_data = database_obj->get_page(url); //has its own locking

        if(page_data == nullptr)
        {
            dbg<<"creating new page"<<std::endl;
            page_data = new struct page_data_s;
        }

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

    if(priority_ctl.fill < PC_UPPER_WATERMARK) { //fill cache
        priority_cache.insert(page);
        cache_housekeeping(PCACHE_INS, url, page_data);

    } else if(page_data->rank > priority_ctl.lowest_entry->rank) {
        cache_housekeeping(PCACHE_RM, url, page_data);
        priority_cache.insert(page);
    } else {
        dbg<<"not inserting page"<<std::endl;
    }

    priority_ctl.rw_mutex.unlock();

    //send to db
    database_obj->put_page(url, page_data);
}

