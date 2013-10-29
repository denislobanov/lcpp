#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <ctime>
#include <unistd.h>         //sleep()
#include <glibmm/ustring.h> //utf-8 strings

#include "crawler_worker.hpp"
#include "parser.hpp"
#include "netio.hpp"
#include "robots_txt.hpp"
#include "ipc_client.hpp"
#include "page_data.hpp"
#include "ipc_common.hpp"
#include "memory_mgr.hpp"

//Local defines
#define DEBUG 1
#define SEED_URL "http://en.wikipedia.org/wiki/Microprocessor"
#define SEED_CREDIT 2048

#if defined(DEBUG)
    #define dbg std::cout<<__FILE__<<"("<<__LINE__<<"): "
    #if DEBUG > 1
        #define dbg_1 std::cout<<__FILE__<<"("<<__LINE__<<"): "
    #else
        #define dbg_1 0 && std::cout
    #endif
    #if DEBUG > 2
        #define dbg_2 std::cout<<__FILE__<<"("<<__LINE__<<"): "
    #else
        #define dbg_2 0 && std::cout
    #endif
#else
    #define dbg 0 && std::cout
    #define dbg_1 0 && std::cout
    #define dbg_2 0 && std::cout
#endif

//development version, makes assumptions until IPC is in place
crawler_worker::crawler_worker(std::vector<struct tagdb_s>& parse_param)
{
    //pretend configuration
    status = READY;
    config.db_path = "./test_db";
    config.user_agent = "lcpp test";
    config.parse_param = parse_param;

    netio_obj = new netio(config.user_agent);
    mem_mgr = new memory_mgr(config.db_path, config.user_agent);

    //test seed to queue
    dbg<<"seed url is: "<<SEED_URL<<" initial credit "<<SEED_CREDIT<<std::endl;
    struct queue_node_s preseed_node;
    preseed_node.url = SEED_URL;
    preseed_node.credit = SEED_CREDIT;
    ipc.send_item(preseed_node);
}

crawler_worker::crawler_worker(void)
{
    //set to idle on entry to main loop
    status = ZOMBIE;
}

crawler_worker::~crawler_worker(void)
{
    delete netio_obj;
    delete mem_mgr;
}

size_t crawler_worker::root_domain(std::string& url)
{
    //consider longest scheme name
    //  01234567
    // "https://" next "/" is at the end of the root url
    dbg<<"url ["<<url<<"] root domain is char 0 -> "<<url.find_first_of("/", 8)<<std::endl;
    return url.find_first_of("/", 8);
}

void crawler_worker::dev_loop(int i) throw(std::underflow_error)
{
    while(--i) {
        dbg<<"loops left: "<<i<<std::endl;

        //get work item
        try {
            queue_node_s work_item = ipc.get_item();
            status = ACTIVE;

            //get memory
            struct page_data_s* page = mem_mgr->get_page(work_item.url);
            Glib::ustring root_url(work_item.url, 0, root_domain(work_item.url));
            dbg<<"root_url ["<<root_url<<"]\n";
            robots_txt* robots = mem_mgr->get_robots_txt(root_url);


            //check robots_txt is valid
            if(std::difftime(std::time(0), robots->last_visit) >= ROBOTS_REFRESH) {
                dbg<<"refreshing robots_txt\n";
                robots->fetch(*netio_obj);
            }

            //can we crawl this page?
            if(!robots->exclude(work_item.url)) {
                dbg<<"can crawl page\n";
                //for dev just sleep. prod should put item back on work queue
                while(std::difftime(std::time(0), robots->last_visit) < robots->crawl_delay) {
                    status = SLEEP;
                    sleep(robots->crawl_delay);
                    dbg<<"crawl delay not reached, sleeping for "<<robots->crawl_delay<<" seconds\n";
                }
                status = ACTIVE;
                robots->last_visit = std::time(0);

                //page rank housekeeping
                page->rank += work_item.credit;

                //parse page
                parser single_parser(work_item.url, config.parse_param);
                single_parser.parse();
                if(!single_parser.data.empty()) {
                    //replaced by that returned from the crawl
                    page->meta.clear();

                    //we need to know the total for page ranking
                    unsigned int linked_pages = 0;
                    for(auto& d: single_parser.data) {
                        //this is a good time to sanitise crawled data
                        sanitize_tag(d);

                        //all "a" tags matched only to "href" attr
                        //so can assume to be link
                        if(d.tag_name.compare("a") == 0)
                            ++linked_pages;
                    }

                    //FIXME: tax page here
                    unsigned int new_credit = page->rank/linked_pages;
                    page->rank = 0;
                    dbg_1<<"linked_pages "<<linked_pages<<" new_credit "<<new_credit<<std::endl;

                    //process data
                    for(auto& d: single_parser.data) {
                        queue_node_s new_item;

                        //all "a" tags matched only to "href" attr
                        if(d.tag_name.compare("a") == 0) {
                            new_item.url = d.attr_data;
                            new_item.credit = new_credit;
                            ipc.send_item(new_item);
                            dbg_2<<"found link ["<<new_item.url<<"]\n";

                        //text is just saved, overwriting previous data
                        } else if(d.tag_name.compare("p") == 0) {
                            page->meta.push_back(d.tag_data);
                            dbg_2<<"found meta\n";

                        //update page title
                        } else if(d.tag_name.compare("title") == 0) {
                            page->title = d.tag_data;
                            dbg_2<<"found title\n";

                        } else {
                            dbg<<"unknown tag ["<<d.tag_name<<"]\n";
                        }
                    }
                }
            } else {
                dbg<<"robots_txt page excluded ["<<work_item.url<<"]\n";
                status = IDLE;

                //page credits to tax instead
            }

            //return memory
            mem_mgr->put_robots_txt(robots, root_url);
            mem_mgr->put_page(page, work_item.url);
        } catch(std::underflow_error& e) {
            std::cerr<<"ipc work queue underrun: "<<e.what()<<std::endl;
            throw std::underflow_error("ipc_client::get_item reports empty");
        }
    }

    status = IDLE;
}

void crawler_worker::main_loop(void)
{
    dbg<<"not yet implemented\n";
}

/* to do
 * use new tagdb_s configuration structur
 * sanitize should check tag/attr v enum, plus tag data etc
 * fix up missing domain, scheme etc
 *  - handle relative links
 */


void crawler_worker::sanitize_tag(struct data_node_s& t)
{
    switch(t.tag_type) {
    case url:
    {
        if(t.tag_name.compare("a") == 0) {
            // FIXME: proper https support, for now convert https to
            //http schemes
            if(t.attr_data.substr(0, 5).compare("https") == 0) {
                dbg_1<<"removing ssl scheme from ["<<t.attr_data<<"]\n";
                t.attr_data.erase(4, 1);
                dbg_2<<"now ["<<t.attr_data<<"]\n";
            } else if(t.attr_data.substr(0, 4).compare("http") != 0) {
                dbg<<"not a valid url ["<<t.attr_data<<"] dropping\n";
            }
        }
        break;
    }

    case title:
    case description:
    case meta:
    case email:
    case image:
    case other:
    default:
        //for now, do nothing
    }
}
