#include <iostream>
#include <vector>
#include <string>
//~ #include <glibmm/ustring.h> //UTF-8 string
#include <rapidxml.hpp>

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

void parser::recurse_child(html_node* cur_node, struct parse_param_s& param)
{
    html_node* child = cur_node->first_node(param.tag.c_str());

    if(child != 0) {
        //get data from all childern nodes
        recurse_child(child, param);

        //get data from all sibling nodes
        recurse_sibling(cur_node, param);

        //save data from this node
        save_node(cur_node, param);
    }
}

void parser::recurse_sibling(html_node* cur_node, struct parse_param_s& param)
{
    html_node* sibling = cur_node->next_sibling(param.tag.c_str());

    if(sibling != 0) {
        recurse_sibling(cur_node, param);

        //get data from this node
        save_node(cur_node, param);
    }
}

void parser::save_node(html_node* node, struct parse_param_s& param)
{
    //should we save attr data too
    if(param.attr.size() > 0) {
        dbg<<"atrribute matching\n";
        if(node->first_attribute(param.attr.c_str()) != 0)
            push_back_node(node, param, true);

    //ignoring tag params, save node anyway
    } else {
        push_back_node(node, param, false);
    }
}

void parser::push_back_node(html_node* node, struct parse_param_s& param, bool attr)
{
    struct data_node_s data_entry;

    dbg<<"saving node\n";
    data_entry.tag_name = param.tag;
    data_entry.tag_data = node->value();
    if(attr)
        data_entry.attr_data = node->first_attribute(param.attr.c_str())->value();

    //save
    data.push_back(data_entry);
}

void parser::parse(std::string data)
{
    std::vector<char> rw_data(data.begin(), data.end());
    rw_data.push_back('\0');

    //do parse
    dbg<<"rapidxml parse\n";
    try {
        doc.parse<rapidxml::parse_declaration_node|
                  rapidxml::parse_no_data_nodes|
                  rapidxml::parse_trim_whitespace
                 >(&rw_data[0]);
    } catch(rapidxml::parse_error &e) {
        std::cerr<<"exception in rapidxml::parse - "<<e.what()<<std::endl;
        exit(-1); //FIXME - this is for debug only. parser should return error
    }

    //find root node "<html>"
    html_node* rnode = doc.first_node("html");
    dbg<<"found root node "<<rnode<<std::endl;

    //process all tags
    for(auto& param: params) {
        dbg<<"trying tag ["<<param.tag<<"]"<<std::endl;
        html_node* cur_node = rnode->first_node(param.tag.c_str());

        if(cur_node != 0) {
            dbg<<"found tag\n";
            
            //check all child nodes
            recurse_child(cur_node, param);

            //and all siblings
            recurse_sibling(cur_node, param);

            //save node data
            save_node(cur_node, param);
        } else {
            dbg<<"cur_node is "<<cur_node<<std::endl;
        }
    }
}

void parser::get_data(std::vector<struct data_node_s>& copy_data)
{
    dbg<<"data length "<<data.size()<<std::endl;
    copy_data = data;

    dbg<<"copy_data size "<<copy_data.size()<<std::endl;
}
