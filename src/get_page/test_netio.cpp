#include <iostream>
#include <fstream>

#include "netio.h"
#include "parser.h"

int main(void)
{
    std::string web_page;
    const std::string search_string = "geeks";
    
    std::cout<<"initialising objects"<<std::endl;
    netio my_netio("lcpp netio test");
    parser title_parser("<title>", "</title>");

    std::cout<<"fetching http://www.geeksaresexy.net/"<<std::endl;
    my_netio.fetch(&web_page, "http://www.geeksaresexy.net/");

    std::cout<<"dumping to test_file.html"<<std::endl;
    std::ofstream stream;
    stream.open("test_file.html");
    if(stream != 0)
        stream<<web_page<<std::endl;

    std::cout<<"parsing.."<<std::endl;
    title_parser.parse(&web_page);
    int i = title_parser.search(&web_page, search_string);
    if(i > 0)
        std::cout<<"title_parser found "<<i<<" occurances of \'"<<search_string<<"\'!"<<std::endl;
    else
        std::cout<<"title_parser failed to find any occurances of \'"<<search_string<<"\'"<<std::endl;
    
    std::cout<<"done"<<std::endl;
    return 0;
}
