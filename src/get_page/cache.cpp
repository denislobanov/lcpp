#include <iostream>
#include <map>
#include <mutex>
#include <chrono>

#include "cache.hpp"
#include "database.hpp"
#include "robots_txt.hpp"
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

cache::cache(void)
{
    page_ctl.fill = 0;
    robots_ctl.fill = 0;
}

cache::~cache(void)
{
    //nop
}

bool cache::get_page_data(struct page_data_s** page_data, std::string& url)
{
    bool in_cache = false;

    //both getting and putting entries in cache have to be atomic
    page_ctl.rw_mutex.lock();

    try {
        *page_data = page_cache.at(url).page;
        page_cache.at(url).timestamp = std::chrono::steady_clock::now();
        in_cache = true;

        dbg<<"got page, desc: "<<(*page_data)->description<<std::endl;
    } catch(const std::out_of_range& e) {
        dbg_1<<" page ["<<url<<"] not in cache\n";
    }

    page_ctl.rw_mutex.unlock();
    return in_cache;
}

bool cache::get_robots_txt(struct page_data_s** robots, std::string& url)
{
    bool in_cache = false;
    //both getting and putting entries in cache have to be atomic
    robots_ctl.rw_mutex.lock();

    try {
        *robots = robots_cache.at(url).robots;
        robots.at(url).timestamp = std::chrono::steady_clock::now();
        in_cache = true;

        dbg<<"got robots\n";
    } catch(const std::out_of_range& e) {
        dbg_1<<" robots_txt for ["<<url<<"] not in cache\n";
    }

    robots_ctl.rw_mutex.unlock();
    return in_cache;
}

bool cache::put_page_data(struct page_data_s* page_data, std::string& url)
{
    bool in_cache = true;

    priority_ctl.rw_mutex.lock();
    //entries already in cache get automatically updated
    try {
        page_cache.at(url).timestamp = std::chrono::steady_clock::now();
        dbg<<"page ["<<url<<"] already in cache, updating\n";
    } catch(const std::out_of_range& e) {
        cache_map_t::iterator pos = page_cache.begin();
        struct cache_entry_s entry;
        entry.page = page_data;
        entry.timestamp = std::chrono::steady_clock::now();

        //pages not in cache will need to be added in, if there's space
        if(page_ctl.fill < PAGE_CACHE_MAX) {
            dbg<<"space in cache to insert page ["<<url<<"]\n";
            page_cache.insert(pos, std::pair<std::string, struct cache_entry_s>(url, entry));
            ++page_ctl.fill;

            //kick off cache pruning job
            prune_cache(page_cache, page_ctl);
        } else {
            //insert only if page.rank > oldest cached page.rank
            if(page_data->rank > page_cache.rbegin()->second.page->rank) {
                dbg<<"page ["<<url<<"] outranks oldest cache entry: "<<page->rank<<" v "<<page_cache.rbegin()->second.page->rank<<std::endl;
                page_cache.erase(page_cache.rbegin());

                page_cache.insert(pos, std::pair<std::string, struct cache_entry_s>(url, entry));
            } else {
                dbg<<"not putting page in cache\n";
                in_cache = false;
            }
        }
    }

    priority_ctl.rw_mutex.unlock();
    return in_cache;
}

bool cache::put_robots_txt(robots_txt* robots, std::string& url)
{
    bool in_cache = true;

    robots_ctl.rw_mutex.lock();
    //entries already in cache get automatically updated
    try {
        robots_cache.at(url).timestamp = std::chrono::steady_clock::now();
        dbg<<"robots_txt ["<<url<<"] already in cache, updating\n";
    } catch(const std::out_of_range& e) {
        cache_map_t::iterator pos = robots_cache.begin();
        struct cache_entry_s entry;
        entry.robots = robots;
        entry.timestamp = std::chrono::steady_clock::now();

        //pages not in cache will need to be added in, if there's space
        if(robots_ctl.fill < ROBOTS_CACHE_MAX) {
            dbg<<"space in cache to insert robots_txt ["<<url<<"]\n";
            robots_cache.insert(pos, std::pair<std::string, struct cache_entry_s>(url, entry));
            ++robots_ctl.fill;

            //kick off cache pruning job
            prune_cache(robots_cache, robots_ctl);
        } else {
            //kick oldest item if stale
            dbg<<"FIXME\n";
            in_cache = false;
        }
    }

    robots_ctl.rw_mutex.unlock();
    return in_cache;
}

void cache::prune_cache(cache_type t)
{
    cache_map_t& cm;
    cache_ctl_s& ctl;
    int max_fill, reserve;

    if(t == PAGE) {
        cm = page_cache;
        ctl = page_ctl;
        max_fill = PAGE_CACHE_MAX;
        reserve = PAGE_CACHE_RES;
    } else {
        cm = robots_cache;
        ctl = robots_ctl;
        max_fill = ROBOTS_CACHE_MAX;
        reserve = ROBOTS_CACHE_RES;
    }

    //FIXME: put this into a thread
    while(ctl.fill-(max_fill-reserve) > 0) {
        //free memory
        delete cm.rbegin()->second.page;
        delete cm.rbegin()->second.robots;

        cm.erase(cm.rbegin());
        --ctl.fill;
    }
}
