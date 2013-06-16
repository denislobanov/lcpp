#if !defined(PARSER_H)
#define PARSER_H

#include <iostream>
#include <vector>
#include <rapidxml.hpp>
//this will be implemented when basic parsing works
//~ #include <glibmm/ustring.h> //UTF-8 string

//data returned from crawl
struct data_node_s {
    std::string tag_name;                 //tag name crawled
    std::string tag_data;                 //data contained within tag
    std::string attr_data;   //value of each tag attribute
};

//control parsing behaviour
enum parent_tag_e {
    HTML_HEAD,
    HTML_BODY
};

struct parse_param_s {
    parent_tag_e parent_tag;
    std::string tag;                    //tag to match
    std::string attr;                   //match tags with a certain attribute only
};

typedef rapidxml::xml_node<> html_node;

class parser
{
    public:
    parser(std::vector<struct parse_param_s>& parse_param);
    ~parser(void);

    //reconfigure parser
    void configure(std::vector<struct parse_param_s>& parse_param);
    
    //walks the document tree, parsing based on configuration
    void parse(std::string data);

    //copies internal data to user referenced mem
    void get_data(std::vector<struct data_node_s>& copy_data);
  
    private:
    std::vector<struct parse_param_s> params;
    std::vector<struct data_node_s> data;

    rapidxml::xml_document<std::string::value_type> doc;

    void recurse_child(html_node* cur_node, struct parse_param_s& param);
    void recurse_sibling(html_node* cur_node, struct parse_param_s& param);
    void save_node(html_node* node, struct parse_param_s& param);
    void push_back_node(html_node* node, struct parse_param_s& param, bool attr);
};

#endif
