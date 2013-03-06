#include <iostream>
#include <fstream>

#include "netio.h"

int main(void)
{
    std::string web_page;

    std::cout<<"initialising objects"<<std::endl;
    netio my_netio("lcpp netio test");

    std::cout<<"fetching http://www.geeksaresexy.net/"<<std::endl;
    my_netio.fetch(&web_page, "http://www.geeksaresexy.net/");

    std::cout<<"dumping to test_file.html"<<std::endl;
    std::ofstream stream;
    stream.open("test_file.html");
    if(stream != 0)
        stream<<web_page<<std::endl;

    std::cout<<"done"<<std::endl;
    return 0;
}
