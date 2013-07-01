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
#define DEBUG 5

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
    single_parser = new parser(parse_param);
}

crawler_mgr::~crawler_mgr(void)
{
    delete single_parser;
}

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
        struct page_data_s* root_page = mem_mgr->get_page(root_url);

        dbg<<"root_page.url ["<<root_page->url<<"]\n";

        //check robots_txt
        if(std::difftime(std::time(0), root_page->last_visit) >= ROBOTS_REFRESH) {
            dbg<<"refreshing robots_txt\n";
            root_page->robots->fetch(*netio_obj);
        }

        //crawl delay
        while(std::difftime(std::time(0), root_page->last_visit) < root_page->crawl_delay) {
            status = SLEEP;
            sleep(root_page->crawl_delay);  //FIXME find other method
            dbg<<"crawl delay not reached, sleeping for "<<root_page->crawl_delay<<" seconds\n";
        }
        status = ACTIVE;

        //can we crawl this page?
        if(root_page->robots->exclude(work_item.url)) {
            dbg<<"robots_txt page excluded ["<<work_item.url<<"]\n";

            //forward credits got to tax instead
            queue_obj->tax_in(work_item.credit);
            status = IDLE;
        } else {
            dbg_1<<"can crawl page\n";
            page->rank += work_item.credit;
            if(netio_obj->fetch(&data, work_item.url)) {
                dbg<<"netio_obj->fetch OK\n";
            } else {
                std::cerr<<"netio_obj->fetch FAILED at url ["<<work_item.url<<"]\n";
                exit(-1);
            }

            //parse page
            single_parser->parse(data);
            std::vector<struct data_node_s> parse_data;
            single_parser->get_data(parse_data);

            //iterate list, first we need to count totals (for tax)
            unsigned int linked_pages = 0;
            for(auto& it: parse_data) {
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
            for(auto& it: parse_data) {
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

        //put memory
        mem_mgr->put_page(root_url, root_page);
        mem_mgr->put_page(work_item.url, page);
    }

    status = IDLE;
}
