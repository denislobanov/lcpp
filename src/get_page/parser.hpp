#if !defined(PARSER_H)
#define PARSER_H

#include <iostream>
#include <vector>

class parser
{
    public:
    /**
     *  initialises a keyword specific parser.
     *
     * start_type and end_type create a 'grid' limiting search/extraction
     * to specific areas of data
     */
    parser(std::string start_type, std::string end_type);
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
    struct data_grid_s {
        size_t start, end;
    };

    std::string tag_open;
    std::string tag_close;
    size_t tag_open_offset;
    std::vector<struct data_grid_s> data_grid;  //exlude data outside of grid

    void print_debug(std::string message);
};

#endif
