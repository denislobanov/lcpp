#if !defined(PARSER_H)
#define PARSER_H

#include <iostream>
#include <vector>
#include <glibmm/ustring.h> //UTF-8 string

class netio;

//data returned from crawl
struct data_node_s {
    std::string tag_name;                   //tag name crawled
    Glib::ustring tag_data;                 //data contained within tag
    std::vector<Glib::ustring> attr_data;   //value of each tag attribute
};

//control parsing behaviour
struct parse_param_s {
    std::string tag;                    //tag to match
    std::vector<std::string> attr;      //match tags with certain attributes only
};

class parser: public xmlpp::SaxParser
{
    public:
    parser(netio* netio_object, std::vector<struct parse_param_s>& parse_param);
    ~parser(void);

    //reconfigure parser
    void configure(std::vector<struct parse_param_s>& parse_param);
    
    //walks the document tree, parsing based on configuration
    void parse(std::string url, std::vector<struct data_node_s>& parse_data);

    protected:  //libxml++ SAX parsing overrides
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
    std::vector<struct parse_param_s> param;
    std::vector<struct data_node_s> data;
    netio* netio_obj;
};

#endif
