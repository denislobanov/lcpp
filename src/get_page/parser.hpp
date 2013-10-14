#if !defined(PARSER_H)
#define PARSER_H

#include <iostream>
#include <vector>
#include <libxml/HTMLparser.h>  //libxml2
#include <libxml/xpath.h>
#include <glibmm/ustring.h>

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
    Glib::ustring tag;                    //tag to match
    Glib::ustring attr;                   //match tags with a certain attribute only
    Glib::ustring xpath;                  //xpath to match node
};

class parser
{
    public:
    parser(std::vector<struct parse_param_s>& parse_param);
    ~parser(void);

    //reconfigure parser
    void reconfigure(std::vector<struct parse_param_s>& parse_param);

    //walks the document tree, parsing based on configuration
    void parse(Glib::ustring url);

    //copies internal data to user referenced mem
    void get_data(std::vector<struct data_node_s>& copy_data);

    private:
    std::vector<struct parse_param_s> params;
    std::vector<struct data_node_s> data;

    //libxml2
    int hopts; //hard options
    htmlDocPtr doc;
    xmlXPathObjectPtr tags;

    void save_nodes(struct parse_param_s& param);
};

#endif
