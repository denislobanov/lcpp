#include <iostream>
#include <fstream>

#include "crawler_process.h"
#include "netio.h"

int main(void)
{
    search_grid keywords;       // search grid of nodes for keywords
    search_grid urls;           // search grid of nodes for urls (for output_urls)
    search_grid_node_s node;    // a single search grid node, used to compose grids
    page_node_s page_node;      //contains crawl output per page
    
    std::queue<std::string> output_urls;    //more urls to crawl
    std::string test_url = "www.geksaresexy.net";   //seed

    netio my_netio("lcpp crawler_process unit test");   //not thread safe, we own it

    //create search grids
    node.start = "<img src=\"";
    node.end = "\"";
    keywords.push_back(node);
    node.start = "<a href=\"";
    node.end = "\"";
    keywords.push_back(node);

    //same thing for urls..
    urls.push_back(node);

    //create a crawler instance;
    crawler_process test_crawler(&my_netio, &output_urls, keywords, urls);

    enum worker_status status;
    test_crawler.get_status(status);
    if(status != IDLE) {
        std::cerr<<"test_crawler.get_status != IDLE"<<std::endl;
    } else {
        std::cout<<"running crawler_process"<<std::endl;

        test_crawler.crawl(test_url, page_node);
        std::cout<<"crawler keywords:\n"<<std::endl;
        for(std::vector<std::string>::iterator it = page_node.meta.begin();
            it != page_node.meta.end(); ++it)
        {
            std::cout<<*it<<std::endl;
        }
    }

    return 0;
}
