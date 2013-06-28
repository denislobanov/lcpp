#if !defined(PARSER_H)
#define PARSER_H

#include <iostream>
#include <vector>
#include <glibmm/ustring.h> //UTF-8 string
#include <libxml++/libxml++.h>

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

class parser: public xmlpp::SaxParser
{
    public:
    parser(std::vector<struct parse_param_s>& parse_param);
    ~parser(void);

    //reconfigure parser
    void configure(std::vector<struct parse_param_s>& parse_param);

    //copies internal data to user referenced mem
    void get_data(std::vector<struct data_node_s>& copy_data);

    //SAX interface
    protected:
    virtual void on_start_document();
    virtual void on_end_document();
    virtual void on_start_element(const Glib::ustring& name, const AttributeList& properties);
    virtual void on_end_element(const Glib::ustring& name);
    virtual void on_characters(const Glib::ustring& characters);
    virtual void on_comment(const Glib::ustring& text);
    virtual void on_warning(const Glib::ustring& text);
    virtual void on_error(const Glib::ustring& text);
    virtual void on_fatal_error(const Glib::ustring& text);

    private:
    std::vector<struct parse_param_s> params;
    std::vector<struct data_node_s> data;

    struct data_node_s current_node;
};

#endif
