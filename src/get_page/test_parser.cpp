#include <iostream>
#include <fstream>
#include <streambuf>
#include <vector>

#include "parser.h"

int main(void)
{
    const std::string search_string = "geeks";

    std::cout<<"loading test_file.html"<<std::endl;
    std::ifstream file("full_test_file.html");
    std::string web_page;

    file.seekg(0, std::ios::end);   
    web_page.reserve(file.tellg());
    file.seekg(0, std::ios::beg);
    
    web_page.assign((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    
    std::cout<<"initialising objects"<<std::endl;
    parser title_parser("title=\"", "\"");

    std::cout<<"parsing.."<<std::endl;
    title_parser.parse(web_page);

#if 0
    int i = title_parser.search(web_page, search_string);
    if(i > 0)
        std::cout<<"title_parser found "<<i<<" occurances of \'"<<search_string<<"\'!"<<std::endl;
    else
        std::cout<<"title_parser failed to find any occurances of \'"<<search_string<<"\'"<<std::endl;
#endif

    std::cout<<"searching for keywords.."<<std::endl;
    std::vector<std::string> keywords;
    unsigned int count =  title_parser.extract_separated(web_page, keywords, " \n\t", true);
    unsigned int our_count = 0;
    std::cout<<"---"<<std::endl;
    for(std::vector<std::string>::iterator limit = keywords.begin();
            limit != keywords.end(); ++limit, ++our_count)
        std::cout<<*limit<<std::endl;
    std::cout<<"---"<<std::endl;
    
    std::cout<<count<<" keywords reported, "<<our_count<<" keywords found"<<std::endl;

    std::cout<<"done"<<std::endl;
    return 0;
}
