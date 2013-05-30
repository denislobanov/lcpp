#if !defined(PARSER_H)
#define PARSER_H

#include <iostream>
#include <vector>

//best explained in the example of an extracted link:
//  content:    page url
//  meta:       keywords from linking site
struct data_node_s {
    std::string content;
    std::vector<std::string> meta;
};

//best explained in the example of searching for a
struct search_node_s {
    std::string name;
    //wip
};

struct search_param_s {
    std::string parent_tag;         //consider tags from one parent only
    std::vector<std::string> attr;  //match tags with certain attributes only
    std::string tag_name;
};

class parser
{
    public:
    parser(struct search_param_s& search_param);
    ~parser(void);

    /**
     * extracts keywords from within parse data grid
     */
    unsigned int extract(std::string& data, std::vector<std::string>& token_set);
    unsigned int extract_separated(std::string& data, std::vector<std::string>& token_set, std::string deliminator, bool strip_empty);
    /**
     * search for keywords within data grid
     */
    unsigned int search(std::string& data, std::string keyword);
    /**
     * create data grid. this is not done by the constructor as parsers should
     * be reused
     */
    void parse(std::string& data);
    void prepair(std::string& data);

    private:
    struct search_param_s param;
};

#endif
