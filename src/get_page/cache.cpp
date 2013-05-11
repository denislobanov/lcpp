#include <iostream>

#include "cache.hpp"

//Local defines
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

cache::cache(netio* netio_object, database* database_object)
{

}

cache::cache(void)
{

}

struct page_data_s* cache::get_page(std::string& url)
{
    struct page_data_s* page_node;

    //hash url
    
    //check priority cache first

    //then timed access

    //try db

    //allocate a new page object
}
