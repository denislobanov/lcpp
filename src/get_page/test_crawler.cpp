#include <iostream>
#include <fstream>

#include "crawler_worker.hpp"
#include "netio.hpp"
#include "page_data.hpp"

#define USER_AGENT "lcpp_test"
#define DATABASE_PATH "./test_db"
#define SEED_URL "http://www.xmlsoft.org/"
#define SEED_CREDIT 10
#define CRAWL_LOOPS 10

using std::cout;
using std::endl;

int main(void)
{
    netio test_netio(USER_AGENT);
    qc_config qcc = {0};
    queue_client test_queue(qcc);
    memory_mgr test_mmgr(DATABASE_PATH, USER_AGENT);

    //create parser config
    std::vector<struct parse_param_s> parse_param;
    struct parse_param_s param;

    param.tag = "a";
    param.attr = "href";
    parse_param.push_back(param);

    param.tag = "title";
    param.attr = "";
    parse_param.push_back(param);

    param.tag = "p";
    param.attr = "";
    parse_param.push_back(param);

    cout<<"creating crawler_worker"<<endl;
    crawler_worker test_crawler(&test_netio, &test_queue, &test_mmgr, parse_param);

    //preseed queue
    cout<<"seed url is: "<<SEED_URL<<" initial credit "<<SEED_CREDIT<<endl;
    struct queue_node_s preseed_node;
    preseed_node.url = SEED_URL;
    preseed_node.credit = SEED_CREDIT;
    test_queue.send(preseed_node);

    worker_status crawler_status;
    test_crawler.get_status(crawler_status);
    if(crawler_status == IDLE) {
        cout<<"begin crawl of "<<CRAWL_LOOPS<<" pages\n";
        test_crawler.loop(CRAWL_LOOPS);

        //analyse data
        cout<<"test queue contents are\n";
        int i = 0;
        while(true) {
            struct queue_node_s node = test_queue.fetch();
            cout<<"node "<<i<<" url ["<<node.url<<"] credit "<<node.credit<<endl;
        }
    } else {
        std::cerr<<"FATAL ERROR: crawler_worker status is not IDLE (is: "<<crawler_status<<")\n";
    }

    return 0;
}
