#include <iostream>
#include <vector>
#include <string>
//~ #include <glibmm/ustring.h> //UTF-8 string
#include <rapidxml.hpp>

#include "parser.hpp"
#include "netio.hpp"

//Local defines
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

parser::parser(std::vector<struct parse_param_s>& parse_param)
{
    params = parse_param;
}

parser::~parser(void)
{
    // ?
}

void parser::configure(std::vector<struct parse_param_s>& parse_param)
{
    params = parse_param;
}

void parser::get_data(std::vector<struct data_node_s>& copy_data)
{
}

void parser::on_start_document()
{
    //clear internal data
}

void parser::on_end_document()
{
}

void parser::on_start_element(const Glib::ustring& name, const AttributeList& properties)
{
}

void parser::on_end_element(const Glib::ustring& name)
{
    
}

void parser::on_characters(const Glib::ustring& characters)
{
}

void parser::on_comment(const Glib::ustring& text)
{
}

void parser::on_warning(const Glib::ustring& text)
{
}

void parser::on_error(const Glib::ustring& text)
{
}

void parser::on_fatal_error(const Glib::ustring& text)
{
}

