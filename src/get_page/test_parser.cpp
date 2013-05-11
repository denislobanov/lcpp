#include <iostream>
#include <fstream>
#include <streambuf>
#include <vector>

#include "parser.hpp"

int main(void)
{
    const std::string search_string = "geeks";

    std::cout<<"loading test_file.html"<<std::endl;
    std::ifstream file("test_file.html");
    std::string web_page;

    file.seekg(0, std::ios::end);
    web_page.reserve(file.tellg());
    file.seekg(0, std::ios::beg);

    web_page.assign((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());

    std::cout<<"initialising objects"<<std::endl;
    parser link_parser("<a href=\"", "\"");
    parser img_parser("<img src=\"", "\"");

    std::cout<<"parsing.."<<std::endl;
    link_parser.parse(web_page);
    img_parser.parse(web_page);

#if 0
    int i = title_parser.search(web_page, search_string);
    if(i > 0)
        std::cout<<"title_parser found "<<i<<" occurances of \'"<<search_string<<"\'"<<std::endl;
    else
        std::cout<<"title_parser failed to find any occurances of \'"<<search_string<<"\'!"<<std::endl;
#endif

    std::cout<<"extracting data.."<<std::endl;
    std::vector<std::string> http_links;
    std::vector<std::string> img_links;

    unsigned int http_link_count = link_parser.extract(web_page, http_links);
    unsigned int img_link_count = img_parser.extract(web_page, img_links);

    unsigned int our_http_count = 0;
    unsigned int our_img_count = 0;
    std::cout<<"==="<<std::endl;

    for(std::vector<std::string>::iterator limit = http_links.begin();
            limit != http_links.end(); ++limit, ++our_http_count)
        std::cout<<*limit<<std::endl;

    std::cout<<"---"<<std::endl;

    for(std::vector<std::string>::iterator limit = img_links.begin();
            limit != img_links.end(); ++limit, ++our_img_count)
        std::cout<<*limit<<std::endl;

    std::cout<<"==="<<std::endl;

    std::cout<<http_link_count<<" page links reported, "<<our_http_count<<" page links found"<<std::endl;
    std::cout<<img_link_count<<" image links reported, "<<our_img_count<<" image links found"<<std::endl;
    std::cout<<"done"<<std::endl;
    return 0;
}
