#include <iostream>
#include <vector>
#include <string>

#include "parser_mgr.hpp"





//Local defines
#define DEBUG 2

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

parser_mgr::parser_mgr(std::vector<struct parse_param_s>& parse_param)
{
    configure(parse_params);
}

parser_mgr::~parser_mgr(void)
{
    // ?
}

void parser_mgr::configure(std::vector<struct parse_param_s>& parse_param)
{
    params = parse_param;
}

void parser_mgr::save_node(html_node& node, struct parse_param_s& param)
{

}

void parser_mgr::parse(std::string& data)
{

}

void parser_mgr::get_data(std::vector<struct data_node_s>& copy_data)
{
    dbg<<"data length "<<data.size()<<std::endl;
    copy_data = data;

    dbg<<"copy_data size "<<copy_data.size()<<std::endl;
}
