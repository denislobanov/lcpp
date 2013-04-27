#include <iostream>
#include <ctime>

#include "robots_txt.hpp"
#include "netio.h"

robots_txt::robots_txt(netio& netio_obj, std::string user_agent, std::string root_domain)
{
    agent_name = user_agent;
    domain = root_domain;
    
    std::string temp_data;
    netio_obj.fetch(&temp_data, domain+"/robots.txt");
    parse(temp_data);
}

robots_txt::~robots_txt(void)
{
    //?
}

void robots_txt::refresh(netio& netio_obj)
{
    std::string temp_data;
    
    exclusion_list.clear();
    netio_obj.fetch(&temp_data, domain+"/robots.txt");
    parse(temp_data);
}

bool robots_txt::excluded_dir(std::string& path)
{
    if(!can_crawl)
        return false;
    else
        return false; //WIP
}

//checks if line is a comment;
size_t robots_txt::line_is_comment(std::string& data, size_t& pos)
{
    if(pos > 0)
        if(data[--pos] == "#")
            return true;
            
    return false;
}

//finds end-of-line (eol,  '\n'), subtracts pos
size_t robots_txt::line_length(std::string& data, size_t pos)
{
    return data.find_first_of("\n", pos) - pos;
}

bool robots_txt::match_agent(std::string& data, size_t pos, size_t eol)
{
    //ignore first whitespace after 'User-agent:'
    if(data[++pos] == "*")
        return true;
    
    size_t res = data.compare(pos, eol, agent_name);
    if((res == 0)||(res == agent_name.size())
        return true;
    else
        return false;
}

//should be called after a 'User-agent:' field has been matched and identified
bool robots_txt::process_instruction(std::string& data, pos, eol)
{
    size_t inst_pos = pos;

    while(inst_pos != std::string::npos)
    {
        inst_pos = data.find(""
}

void robots_txt::parse(std::string& data)
{
    //site does not have a robots.txt or failed to retrieve one..
    if(data.size() == 0) {
        // set defaults
        can_crawl = true;
        crawl_delay_time = 1;   //be nice  :)
    } else {
        // parse
        size_t last_pos = 0;
        while(last_pos != std::string::npos) {
            size_t pos = data.find("User-agent:", last_pos);

            //found something
            if(pos != std::string::npos) {
                if(!line_is_comment(data, pos)) {
                    //find line length (tokenize by '\n')
                    size_t eol = line_length(data, pos);
                    
                    //is this for us?
                    if(match_agent(data, pos, eol))
                        while(process_instruction
                }
            }

            last_pos = ++pos;
        }
        
    }
}

time_t robots_txt::crawl_delay(void)
{
    return crawl_delay_time;
}

void robots_txt::import_exclusions(std::vector<std::string>& data)
{
    data = exclusion_list;
}

void robots_txt::export_exclusions(std::vector<std::string>& data)
{
    exclusion_list = data;
}
    



