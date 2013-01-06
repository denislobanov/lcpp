#include <iostream>

#include "netio.h"

int main(void)
{
    std::cout<<"initialising netio object"<<std::endl;
    netio my_netio("lcpp netio test");

    std::cout<<"configuring without debug... ";
    if(my_netio.default_config(false))
        std::cout<<"success"<<std::endl;
    else
        std::cout<<"fail"<<std::endl;

    std::cout<<"resetting configuration"<<std::endl;
    my_netio.reset_config();

    //~ std::cout<<"configuring with debug... ";
    //~ if(my_netio.default_config(true))
        //~ std::cout<<"success"<<std::endl;
    //~ else
        //~ std::cout<<"fail"<<std::endl;

    std::cout<<"fetching https://duckduckgo.com/"<<std::endl;
    my_netio.fetch("https://duckduckgo.com/");

    std::cout<<"shutting down"<<std::endl;
    return 0;
}
