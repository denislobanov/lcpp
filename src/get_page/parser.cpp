#include <iostream>
#include <vector>
#include <string>
#include <glibmm/ustring.h> //UTF-8 string
#include <libxml++/libxml++.h>

#include "parser.hpp"
#include "netio.hpp"

//Local defines
#define DEBUG 2

#if (defined(DEBUG))&&(DEBUG > 2)
#include <fstream>
#endif

#if defined(DEBUG)
    #define dbg std::cout<<__FILE__<<"("<<__LINE__<<"): "
    #if DEBUG > 1
        #define dbg_2 std::cout<<__FILE__<<"("<<__LINE__<<"): "
    #else
        #define dbg_2 0 && std::cout
    #endif
#else
    #define dbg 0 && std::cout
    #define dbg_2 0 && std::cout
#endif

parser::parser(std::vector<struct parse_param_s>& parse_param)
{
    params = parse_param;

    //hardwired config
    dom_parser.set_validate(false);
    dom_parser.set_substitute_entities(true);
    dom_parser.set_throw_messages(false);
}

parser::~parser(void)
{
    // ?
}

void parser::configure(std::vector<struct parse_param_s>& parse_param)
{
    params = parse_param;
}

void parser::save_node(html_node& node, struct parse_param_s& param)
{
    struct data_node_s data_entry;

    dbg_2<<"saving node\n";
    const xmlpp::ContentNode* node_content = dynamic_cast<const xmlpp::ContentNode*>(&node);
    const xmlpp::Element* element_node = dynamic_cast<const xmlpp::Element*>(&node);

    data_entry.tag_name = param.tag;
    if(node_content)
        data_entry.tag_data = node_content->get_content();
    dbg_2<<"saving attribute\n";
    if(element_node && param.attr.size() > 0)
        data_entry.attr_data = element_node->get_attribute_value(param.attr);

    //save
    dbg_2<<"saving data\n";
    data.push_back(data_entry);
}

void parser::parse(std::string& data)
{
    //crutch until we convert to glib::ustring
    std::vector<unsigned char> rw_data(data.begin(), data.end());
    rw_data.push_back('\0');

    //Validate document before parsing


    //DOM parse
    dbg<<"dom_parser parse\n";
    try {
        dom_parser.parse_file(data);
    } catch(const std::exception& e) {
        std::cerr<<"exception in libxml++ dom_parser.parse - "<<e.what()<<std::endl;
        exit(-1); //FIXME - this is for debug only. parser should return error
    }

    if(dom_parser) {
        dbg<<"parse succesful\n";

        html_node* rnode = dom_parser.get_document()->get_root_node();
        dbg<<"found root node "<<rnode->get_path()<<std::endl;

        //process all tags
        for(auto& param: params) {
            dbg_2<<"generating xpath\n";
            std::string xpath = "//" + param.tag;
            if(param.attr.size() > 0)
                xpath += "[@" + param.attr + "]";

            dbg_2<<"matching xpath ["<<xpath<<"]"<<std::endl;
            for(auto& node: rnode->find(xpath)) {
                save_node(*node, param);
            }

        }
    }
}

void parser::get_data(std::vector<struct data_node_s>& copy_data)
{
    dbg<<"data length "<<data.size()<<std::endl;
    copy_data = data;

    dbg<<"copy_data size "<<copy_data.size()<<std::endl;
}
