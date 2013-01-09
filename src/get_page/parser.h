#if !defined(PARSER_H)
#define PARSER_H

#include <iostream>
#include <vector>

class parser
{
    public:
    parser(std::string start_type, std::string end_type);
    ~parser(void);

    std::vector<std::string> keywords(std::string* data);
    unsigned int search(std::string* source, std::string keyword);
    void parse(std::string* data);
    
    private:
    struct data_grid_s {
        size_t start, end;
    };

    std::string tag_open;
    std::string tag_close;
    std::vector<struct data_grid_s> data_grid;
};

#endif
