#if !defined(PARSER_MGR_H)
#define PARSER_MGR_H

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
    Glib::ustring tag;                    //tag to match
    Glib::ustring attr;                   //match tags with a certain attribute only
    Glib::ustring xpath;                  //xpath to match node
};

typedef xmlpp::Node html_node;

class parser_mgr
{
    public:
    parser_mgr(std::vector<struct parse_param_s>& parse_param);
    ~parser_mgr(void);

    //reconfigure parser_mgr
    void configure(std::vector<struct parse_param_s>& parse_param);

    //walks the document tree, parsing based on configuration
    void parse(std::string& data);

    //copies internal data to user referenced mem
    void get_data(std::vector<struct data_node_s>& copy_data);

    private:
};

#endif
