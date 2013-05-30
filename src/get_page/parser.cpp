#include <iostream>
#include <vector>
#include <string>

#include "parser.hpp"

//Local defines
#define DEBUG 1

#if (defined(DEBUG))&&(DEBUG > 2)
#include <fstream>
#endif

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

parser::parser(struct search_param_s& search_param)
{
    param = search_param;   //copy
}

parser::~parser(void)
{
    // ?
}
