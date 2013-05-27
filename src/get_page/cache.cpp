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

cache::cache(void)
{
    priority_ctl.lowest_entry = nullptr;
    priority_ctl.fill = 0;
}

cache::~cache(void)
{
    //nop
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
        page->url = url;    //we rely on this, so to make sure its right..
        dbg<<"priority_ctl.fill "<<priority_ctl.fill<<std::endl;
        break;

    case PCACHE_RM:
    {
        //remove existing lowest entry from cache
        if(!priority_cache.erase(priority_ctl.lowest_entry->url)) {
            std::cerr<<"failed to delete item at key["<<priority_ctl.lowest_entry->url<<"]"<<std::endl;
            exit(-1);
        }
        delete priority_ctl.lowest_entry;

        //quicksort
        priority_ctl.lowest_entry = page;
        priority_ctl.lowest_entry->url = url;    //in case it wasnt set

        //find new lowest entry
        for(auto& node: priority_cache) {
            if(node.second->rank < priority_ctl.lowest_entry->rank) {
                dbg<<"rank "<<node.second->rank<<" lowest entry, was "<<priority_ctl.lowest_entry->rank<<std::endl;
                priority_ctl.lowest_entry = node.second;
            }
        }
        break;
    }
    
    default:
        std::cerr<<"cache::cache_housekeeping_nt given unknown task"<<std::endl;
        break;
    }
}

bool cache::get_page_data(struct page_data_s** page_data, std::string& url)
{
    bool page_in_cache = false;

    //both getting and putting pages in cache have to be atomic
    priority_ctl.rw_mutex.lock();

    cache_map_t::iterator pri_node = priority_cache.find(url);
    if(pri_node != priority_cache.end()) {
        *page_data = pri_node->second;

        //lock page access before returning page
        //to make sure that multiple access to a cache pages does not occure
        (*page_data)->access_lock.lock();

        dbg<<"got page, desc: "<<(*page_data)->description<<std::endl;
        page_in_cache = true;
    }

    priority_ctl.rw_mutex.unlock();
    return page_in_cache;
}

bool cache::put_page_data(struct page_data_s* page_data, std::string& url)
{
    std::pair<std::string, struct page_data_s*> page (url, page_data);
    bool page_in_cache = true;

    priority_ctl.rw_mutex.lock();
    
    if(priority_ctl.fill < PC_UPPER_WATERMARK) { //fill cache
        dbg<<"priority_ctl.fill < PC_UPPER_WATERMARK\n";
        priority_cache.insert(page);
        cache_housekeeping(PCACHE_INS, url, page_data);

    } else if(page_data->rank > priority_ctl.lowest_entry->rank) {
        dbg<<"page_data->rank > priority_ctl.lowest_entry->rank\n";
        priority_cache.insert(page);
        cache_housekeeping(PCACHE_RM, url, page_data);

    } else {
        dbg<<"not inserting page: rank "<<page_data->rank<<" < "<<priority_ctl.lowest_entry->rank<<std::endl;
        page_in_cache = false;
    }
    priority_ctl.rw_mutex.unlock();

    return page_in_cache;
}

