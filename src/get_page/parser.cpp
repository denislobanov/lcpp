#include <iostream>
#include <vector>
#include <string>
#include <glibmm/ustring.h> //UTF-8 string

#include "parser.hpp"
#include "netio.hpp"

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

parser::parser(netio* netio_object, std::vector<struct parse_param_s>& parse_param): public xmlpp::SaxParser
{
    param = parse_param;
    netio_obj = netio_object;
}

parser::~parser(void)
{
    // ?
}

void parser::configure(std::vector<struct parse_param_s>& parse_param)
{
    param = parse_param;   //copy
}

void parser::parse(std::string url, std::vector<struct data_node_s>& parse_data)
{
    //get data
    std::string page_data;
    netio_obj->fetch(&page_data, url);

    //do parse

    parse_data = data;

}

//SAX interface
void parser::on_start_document()
{
    dbg_1<<"document start\n";
}

void parser::on_end_document()
{
    dbg_1<<"document end\n";
}

void parser::on_start_element(const Glib::ustring& name, const AttributeList& properties)
{
    dbg<<"node name: "<<name<<std::endl;

    //check if its ours
    for(auto& tag : param) {
        if(param.tag == name) {
            data.tag_name = name;
            data.tag_data = 
}
