#include <iostream>
#include <vector>
#include <string>

#include "parser.hpp"

parser::parser(std::string start_type, std::string end_type)
{
    tag_open = start_type;
    tag_close = end_type;
    tag_open_offset = tag_open.size();
    //tag_close_offset is not needed

    // debug
    //~ std::cout<<"tag_open ["<<tag_open<<"]"<<std::endl;
    //~ std::cout<<"tag_open_offset ["<<tag_open_offset<<"]"<<std::endl;
    //~ std::cout<<"tag_close ["<<tag_close<<"]"<<std::endl;
}

parser::~parser(void)
{
    // ?
}

unsigned int parser::search(std::string& data, std::string keyword)
{
    size_t last_pos;
    unsigned int count = 0;

    for(std::vector<struct data_grid_s>::iterator limit = data_grid.begin(); limit != data_grid.end(); ++limit) {
        last_pos = limit->start;

        size_t pos = data.find(keyword, last_pos);
        if(pos != std::string::npos) {
            last_pos = pos;
            ++count;
        }
    }

    return count;
}

//set search limits of interest on page
void parser::parse(std::string& data)
{
    struct data_grid_s element;
    size_t pos  = 0;

    //parsing implies new data, parsers can (should) be resused
    data_grid.clear();
    //~ debug
    //~ std::cout<<"start parsing.."<<std::endl;
    while(true) {
        pos = data.find(tag_open, ++pos);
        if(pos != std::string::npos) {   //found something
            pos += tag_open_offset; //tag name itself should not be part of search
            //~ debug
            //~ std::cout<<"found an opening tag @ "<<pos<<std::endl;
            element.start = pos;
            element.end = data.find(tag_close, pos);
            if(element.end == std::string::npos) {
                std::cerr<<"parser::parse -failed to match tag_close in data!"<<std::endl;
                element.end = element.start;
            } else {
                //~ debug
                //~ std::cout<<"found a closing tag @"<<element.end <<std::endl;
            }

            data_grid.push_back(element);
        } else {
            //~ debug
            //~ std::cout<<"end of data"<<std::endl;
            break;
        }
    }
}

//new interface for parse()
void parser::prepair(std::string& data)
{
    parse(data);
}

//use to extract urls
unsigned int parser::extract(std::string& data, std::vector<std::string>& token_set)
{
    unsigned int count = 0;

    for(std::vector<struct data_grid_s>::iterator limit = data_grid.begin(); limit != data_grid.end(); ++limit, ++count)
        token_set.push_back(data.substr(limit->start, (limit->end - limit->start)));

    return count;
}

//use to extract keywords
unsigned int parser::extract_separated(std::string& data, std::vector<std::string>& token_set, std::string deliminator = " ", bool strip_empty = true)
{
    unsigned int count = 0;
    unsigned int grid_count = 0;

    for(std::vector<struct data_grid_s>::iterator limit = data_grid.begin(); limit != data_grid.end(); ++limit, ++grid_count) {
        size_t last_pos = limit->start;

        while(last_pos < limit->end) {
            size_t pos = data.find_first_of(deliminator, last_pos);
            //~ std::cout<<"grid: "<<grid_count<<" start: "<<limit->start<<" end: "<<limit->end<<std::endl;
            //~ std::cout<<"\tlast pos: "<<last_pos<<" pos: "<<pos<<std::endl;

            //found something
            if(pos != std::string::npos) {
                if(pos != last_pos || !strip_empty)     //not empty token
                    token_set.push_back(data.substr(last_pos, pos-last_pos));

                ++count;
                last_pos = ++pos;
            }
        }
    }

    return count;
}

void parser::print_debug(std::string message)
{
#if defined(DEBUG_LEVEL)
    std::cout<<message<<std::endl;
#endif
}
