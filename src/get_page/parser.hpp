#if !defined(PARSER_H)
#define PARSER_H

#include <iostream>
#include <vector>
#include <glibmm/ustring.h> //UTF-8 string
#include <libxml++.h>

//data returned from crawl
struct data_node_s {
    Glib::ustring tag_name;    //tag name crawled
    Glib::ustring tag_data;    //data contained within tag
    Glib::ustring attr_data;   //value of each tag attribute
};

//control parsing behaviour
enum parent_tag_e {
    HTML_HEAD,
    HTML_BODY
};

struct parse_param_s {
    parent_tag_e parent_tag;              //depricated
    Glib::ustring tag;                    //    tag to match
    Glib::ustring attr;                   //    match tags with a certain attribute only
    Glib::ustring xpath;                  //xpath to match node
};

typedef const xmlpp::Node html_node;

class parser
{
    public:
    parser(std::vector<struct parse_param_s>& parse_param);
    ~parser(void);

    //reconfigure parser
    void configure(std::vector<struct parse_param_s>& parse_param);

    //walks the document tree, parsing based on configuration
    void parse(std::string& data);

    //copies internal data to user referenced mem
    void get_data(std::vector<struct data_node_s>& copy_data);

    private:
    std::vector<struct parse_param_s> params;
    std::vector<struct data_node_s> data;
    xmlpp::DomParser dom_parser;

    void recurse_child(html_node* const cur_node, struct parse_param_s& param);
    void recurse_sibling(html_node* const cur_node, struct parse_param_s& param);
    void save_node(html_node* const node, struct parse_param_s& param);
    void push_back_node(html_node* const node, struct parse_param_s& param, bool attr);
};

#endif
