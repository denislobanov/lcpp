#include <iostream>
#include <map>
#include <unordered_map>
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

void cache::update_timestamp(data_map_t& dm, access_map_t& am, std::string url, std::chrono::steady_clock::time_point new_time)
{
    try {
        //delete old timestamp from access map
        std::chrono::steady_clock::time_point old_time = dm.at(url).timestamp;
        am.erase(am.find(old_time));

        //update data timestamp
        dm.at(url).timestamp = new_time;

        //update access time
        am.insert(std::pair<std::chrono::steady_clock::time_point, std::string>(new_time, url));
    } catch(const std::out_of_range& e) {
        std::cerr<<"could not find ["<<url<<"] in data map or access map. Exception was "<<e.what()<<std::endl;
        //rethrow exception?
        exit(-5);
    }
}

bool cache::get_page_data(struct page_data_s** page_data, std::string& url)
{
    bool in_cache = false;

    //both getting and putting entries in cache have to be atomic
    page_ctl.rw_mutex.lock();

    try {
        *page_data = page_cache.at(url).page;
        update_timestamp(page_cache, page_access, url, std::chrono::steady_clock::now());
        in_cache = true;

        dbg<<"got page, desc: "<<(*page_data)->description<<std::endl;
    } catch(const std::out_of_range& e) {
        dbg_1<<" page ["<<url<<"] not in cache\n";
    }

    page_ctl.rw_mutex.unlock();
    return in_cache;
}

bool cache::put_page_data(struct page_data_s* page_data, std::string& url)
{
    bool in_cache = true;
    std::chrono::steady_clock::time_point new_time = std::chrono::steady_clock::now();

    page_ctl.rw_mutex.lock();
    //entries already in cache get automatically updated
    try {
        update_timestamp(page_cache, page_access, url, new_time);
        dbg<<"page ["<<url<<"] already in cache, updating\n";
    } catch(const std::out_of_range& e) {
        data_map_t::iterator pos = page_cache.begin();
        access_map_t::iterator apos = page_access.begin();
        struct cache_entry_s entry;
        entry.page = page_data;
        entry.timestamp = new_time;
        

        //pages not in cache will need to be added in, if there's space
        if(page_ctl.fill < PAGE_CACHE_MAX) {
            dbg<<"space in cache to insert page ["<<url<<"]\n";
            page_cache.insert(pos, std::pair<std::string, struct cache_entry_s>(url, entry));
            page_access.insert(apos, std::pair<std::chrono::steady_clock::time_point, std::string>(new_time, url));
            ++page_ctl.fill;

            //kick off cache pruning job
            prune_cache(PAGE);
        } else {
            std::string oldest_url = page_access.rbegin()->second;
            
            //insert only if page.rank > oldest cached page.rank
            if(page_data->rank > page_cache.at(oldest_url).page->rank) {
                dbg<<"page ["<<url<<"] outranks oldest cache entry: "<<page_data->rank<<" v "<<page_cache.at(oldest_url).page->rank<<std::endl;
                page_access.erase(page_access.find(page_cache.at(url).timestamp));
                page_cache.erase(page_cache.end());

                page_cache.insert(pos, std::pair<std::string, struct cache_entry_s>(url, entry));
                page_access.insert(apos, std::pair<std::chrono::steady_clock::time_point, std::string>(new_time, url));
            } else {
                dbg<<"not putting page in cache\n";
                in_cache = false;
            }
        }
    }

    page_ctl.rw_mutex.unlock();
    return in_cache;
}

bool cache::get_robots_txt(robots_txt** robots, std::string& url)
{
    bool in_cache = false;
    //both getting and putting entries in cache have to be atomic
    robots_ctl.rw_mutex.lock();

    try {
        *robots = robots_cache.at(url).robots;
        update_timestamp(robots_cache, robots_access, url, std::chrono::steady_clock::now());
        in_cache = true;

        dbg<<"got robots\n";
    } catch(const std::out_of_range& e) {
        dbg_1<<" robots_txt for ["<<url<<"] not in cache\n";
    }

    robots_ctl.rw_mutex.unlock();
    return in_cache;
}

bool cache::put_robots_txt(robots_txt* robots, std::string& url)
{
    bool in_cache = true;
    std::chrono::steady_clock::time_point new_time = std::chrono::steady_clock::now();

    robots_ctl.rw_mutex.lock();
    //entries already in cache get automatically updated
    try {
        update_timestamp(robots_cache, robots_access, url, new_time);
        dbg<<"robots_txt ["<<url<<"] already in cache, updating\n";
    } catch(const std::out_of_range& e) {
        data_map_t::iterator pos = robots_cache.begin();
        access_map_t::iterator apos = page_access.begin();
        struct cache_entry_s entry;
        entry.robots = robots;
        entry.timestamp = new_time;

        //pages not in cache will need to be added in, if there's space
        if(robots_ctl.fill < ROBOTS_CACHE_MAX) {
            dbg<<"space in cache to insert robots_txt ["<<url<<"]\n";
            robots_cache.insert(pos, std::pair<std::string, struct cache_entry_s>(url, entry));
            robots_access.insert(apos, std::pair<std::chrono::steady_clock::time_point, std::string>(new_time, url));
            ++robots_ctl.fill;

            //kick off cache pruning job
            prune_cache(ROBOTS);
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
    data_map_t* dm;
    access_map_t* am;
    cache_ctl_s* ctl;
    int max_fill, reserve;

    if(t == PAGE) {
        dm = &page_cache;
        am = &page_access;
        ctl = &page_ctl;
        max_fill = PAGE_CACHE_MAX;
        reserve = PAGE_CACHE_RES;
    } else {
        dm = &robots_cache;
        am = &robots_access;
        ctl = &robots_ctl;
        max_fill = ROBOTS_CACHE_MAX;
        reserve = ROBOTS_CACHE_RES;
    }

    //FIXME: put this into a thread
    while(ctl->fill-(max_fill-reserve) > 0) {
        //free memory
        delete dm->at(am->rbegin()->second).page;
        delete dm->at(am->rbegin()->second).robots;

        dm->erase(dm->find(am->end()->second));
        am->erase(am->end());
        --ctl->fill;
    }
}
