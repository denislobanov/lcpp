#if !defined(PARSER_H)
#define PARSER_H

#include <iostream>
#include <vector>

class parser
{
    public:
    parser(std::string start_type, std::string end_type);
    ~parser(void);

    unsigned int extract(std::string& data, std::vector<std::string>& token_set);
    unsigned int extract_separated(std::string& data, std::vector<std::string>& token_set, std::string deliminator, bool strip_empty);
    unsigned int search(std::string& data, std::string keyword);
    void parse(std::string& data);
    
    private:
    struct data_grid_s {
        size_t start, end;
    };

    std::string tag_open;
    size_t tag_open_offset;
    std::string tag_close;
    std::vector<struct data_grid_s> data_grid;  //exlude data outside of grid
};

#endif
