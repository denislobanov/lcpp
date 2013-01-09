#include <iostream>
#include <vector>
#include <string>

#include "parser.h"

parser::parser(std::string start_type, std::string end_type)
{
    tag_open = start_type;
    tag_close = end_type;
}

parser::~parser(void)
{

}

unsigned int parser::search(std::string* source, std::string keyword)
{
    size_t last_pos;
    unsigned int count = 0;

    for(std::vector<struct data_grid_s>::iterator limit = data_grid.begin(); limit != data_grid.end(); ++limit) {
        last_pos = limit->start;

        size_t pos = source->find(keyword, last_pos);
        if(pos != std::string::npos) {
            last_pos = pos;
            ++count;
        }
    }

    return count;
}

void parser::parse(std::string* data)
{
    struct data_grid_s element;

    size_t pos  = 0;
    while(true) {
        pos = data->find(tag_open, ++pos);
        
        if(pos != std::string::npos) {   //found something
            std::cout<<"found an opening tag @ "<<pos<<std::endl;
            element.start = pos;
            element.end = data->find(tag_close, pos);
            if(element.end == std::string::npos) {
                std::cerr<<"parser::parse -failed to match tag_close in data!"<<std::endl;
                element.end = 0;
            } else {
                std::cout<<"found a closing tag @"<<element.end <<std::endl;
            }

            data_grid.push_back(element);
        } else {
            break;
        }
    }
}
