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

void parser::recurse_child(html_node* const cur_node, struct parse_param_s& param)
{
    for(html_node* child = cur_node->first_node(); child; child = child->first_node()) {
        //check its siblings too
        recurse_sibling(child, param);

        //a node we're interested in
        if(param.tag.compare(child->name()) == 0)
            save_node(child, param);
    }
}

void parser::recurse_sibling(html_node* const cur_node, struct parse_param_s& param)
{
    dbg<<"resurse_sibling\n";
    //find all sibling nodes
    for(html_node* sibling = cur_node->next_sibling(); sibling; sibling = sibling->next_sibling()) {
        dbg<<"sibling "<<sibling<<std::endl;
        //check its childern too
        recurse_child(sibling, param);

        //a node we're interested in
        if(param.tag.compare(sibling->name()) == 0)
            save_node(sibling, param);
    }
}

void parser::save_node(html_node* const node, struct parse_param_s& param)
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

void parser::push_back_node(html_node* const node, struct parse_param_s& param, bool attr)
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

void parser::parse(std::string& data)
{
    std::vector<char> rw_data(data.begin(), data.end());
    rw_data.push_back('\0');

    //do parse
    dbg<<"rapidxml parse\n";
    try {
        doc.parse<rapidxml::parse_declaration_node
                 |rapidxml::parse_no_data_nodes
                 |rapidxml::parse_trim_whitespace
                 |rapidxml::parse_normalize_whitespace
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
        recurse_child(rnode, param);
    }
}

void parser::get_data(std::vector<struct data_node_s>& copy_data)
{
    dbg<<"data length "<<data.size()<<std::endl;
    copy_data = data;

    dbg<<"copy_data size "<<copy_data.size()<<std::endl;
}
