#include <iostream>
#include <fstream>

#include "crawler_process.h"

int main(void)
{
    search_grid keywords;
    search_grid urls;
    search_grid_node_s node;
    std::queue<std::string> output_urls;

    //create search grids for parsers
    node.start = "<img src=\"";
    node.end = "\"";
    keywords.push_back(node);
    node.start = "<a href=\"";
    node.end = "\"";
    keywords.push_back(node);

    //same thing for urls..
    urls.push_back(node);

    //create a crawler instance;
    crawler_process test_crawler(output_urls, keywords, urls);

    enum worker_status status;
    test_crawler.get_status(status);
    if(status != IDLE)
        std::cerr<<"test_crawler.get_status != IDLE"<<std::endl;
    else
        std::cout<<"tis fine"<<std::endl;

    return 0;
}
