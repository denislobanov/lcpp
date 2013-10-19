#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <functional>
#include <ctime>
#include <unistd.h>

#include "crawler_mgr.hpp"
#include "parser.hpp"
#include "netio.hpp"
#include "robots_txt.hpp"
#include "queue_client.hpp"
#include "page_data.hpp"

//Local defines
#define DEBUG 1

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

crawler_mgr::crawler_mgr(netio* nio, queue_client* qc, memory_mgr* mm, std::vector<struct parse_param_s>& parse_param)
{
    //linked objects
    netio_obj = nio;
    queue_obj = qc;
    mem_mgr = mm;

    //local configure
    status = IDLE;
    param = parse_param;
}

crawler_mgr::~crawler_mgr(void) {}

void crawler_mgr::get_status(enum worker_status &crawler_status)
{
    crawler_status = status;
}

size_t crawler_mgr::root_domain(std::string& url)
{
    //  0123456
    // "http://" next "/" is at the end of the root url
    return url.find_first_of("/", 7);
}

void crawler_mgr::loop(int i)
{
    while(--i) {
        dbg<<"nodes left: "<<i<<std::endl;

        //get work item from queue
        queue_node_s work_item = queue_obj->fetch();
        status = ACTIVE;

        //get memory
        struct page_data_s* page = mem_mgr->get_page(work_item.url);
        std::string root_url(work_item.url, 0, root_domain(work_item.url));
        dbg<<"root_url ["<<root_url<<"]\n";
        robots_txt* robots = mem_mgr->get_robots_txt(root_url);

        //check robots_txt
        if(std::difftime(std::time(0), robots->last_visit) >= ROBOTS_REFRESH) {
            dbg<<"refreshing robots_txt\n";
            robots->fetch(*netio_obj);
        }

        //crawl delay
        while(std::difftime(std::time(0), robots->last_visit) < robots->crawl_delay) {
            status = SLEEP;
            sleep(robots->crawl_delay);  //FIXME find other method
            dbg<<"crawl delay not reached, sleeping for "<<robots->crawl_delay<<" seconds\n";
        }
        status = ACTIVE;

        //can we crawl this page?
        if(robots->exclude(work_item.url)) {
            dbg<<"robots_txt page excluded ["<<work_item.url<<"]\n";

            //forward credits got to tax instead
            queue_obj->tax_in(work_item.credit);
            status = IDLE;
        } else {
            dbg<<"can crawl page\n";
            page->rank += work_item.credit;
            parser single_parser(work_item.url, param);

            // try catch block here
            single_parser.parse();

            //iterate list, first we need to count totals (for tax)
            if(!single_parser.data.empty()) {
                unsigned int linked_pages = 0;
                for(auto& it: single_parser.data) {
                    //all "a" tags matched only to "href" attr
                    //so can assume to be link
                    if(it.tag_name.compare("a") == 0)
                        ++linked_pages;
                }
                unsigned int new_credit = page->rank/linked_pages;
                page->rank = 0;

                dbg_1<<"linked_pages "<<linked_pages<<" new_credit "<<new_credit<<std::endl;

                //extract links and text only
                page->meta.clear(); //clear existing page description data
                for(auto& it: single_parser.data) {
                    queue_node_s new_item;

                    //all "a" tags matched only to "href" attr
                    if(it.tag_name.compare("a") == 0) {
                        new_item.url = it.attr_data;
                        new_item.credit = new_credit;
                        dbg_1<<"found link ["<<new_item.url<<"]\n";

                    //text is just saved, overwriting previous data
                    } else if(it.tag_name.compare("p") == 0) {
                        page->meta.push_back(it.tag_data);
                        dbg_1<<"found meta\n";

                    //update page title
                    } else if(it.tag_name.compare("title") == 0) {
                        page->title = it.tag_data;
                        dbg_1<<"found title\n";

                    } else {
                        dbg<<"unknown tag ["<<it.tag_name<<"]\n";
                    }
                }
            }
        }

        //put memory
        dbg<<"robots_txt to memory\n";
        mem_mgr->put_robots_txt(robots, root_url);
        dbg<<"page to memory\n";
        mem_mgr->put_page(page, work_item.url);
    }

    status = IDLE;
}
