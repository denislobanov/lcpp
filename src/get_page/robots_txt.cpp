#include <iostream>
#include <sstream>  //stringstream for line orientated parsing
#include <ctime>
#include <sstream>  //convert int to string
#include <vector>
#include <algorithm>    //std::remove, to remove '*' from exlusion_list

//FIXME: debug
#include <fstream>

#include "robots_txt.hpp"
#include "netio.h"

robots_txt::robots_txt(netio& netio_obj, std::string user_agent, std::string root_domain)
{
    agent_name = user_agent;
    domain = root_domain;
    process_param = false;  //set to true on matching user-agent by process instruction

    refresh(netio_obj);
}

robots_txt::~robots_txt(void)
{
    //?
}

void robots_txt::refresh(netio& netio_obj)
{
    //~ std::string temp_data;

    //(re)set defaults
    exclusion_list.clear();
    can_crawl = true;
    crawl_delay_time = DEFAULT_CRAWL_DELAY;

    //~ netio_obj.fetch(&temp_data, domain+"/robots.txt");
    std::fstream debug_file;
    debug_file.open("robots.txt");

    std::string temp_data((std::istreambuf_iterator<char>(debug_file)),
                           std::istreambuf_iterator<char>());
    
    parse(temp_data);
}

bool robots_txt::exclude(std::string& path)
{
    if(!can_crawl)
        return true;

    for(std::vector<std::string>::iterator it = exclusion_list.begin();
        it != exclusion_list.end(); ++it)
    {
        size_t ret = path.compare(0, path.size(), *it, 0, it->size());
        if((ret == 0)||(ret == it->size()))
            return true;
    }

    return false;
}

//checks if line is a comment
size_t robots_txt::line_is_comment(std::string& data)
{
    size_t pos = 0;
    size_t eol = data.length();

    if(eol > 0) {
        do {
            if(data.compare(pos, 1, "#") == 0)
                return true;
            else if(data.compare(pos, 1, "\n") == 0)
                return true;
            else if(data.compare(pos, 1, " ") == 0)
                return false;
        } while((data.compare(++pos, 1, " ") == 0)&&(pos < eol));
    }

    return false;
}

static char lower_case(char c)
{
    if(std::isupper(c))
        return std::tolower(c);
    else
        return c;
}

//matches lower case param representation in lc_data, if found pos and eol
//are modified to represent the data after the param without whitespace
//  and returns true
//else returns false, pos and eol unmodified
bool robots_txt::get_param(std::string& lc_data, size_t& pos, size_t& eol, std::string param)
{
    size_t param_length = param.length();

    std::cout<<"robots_txt::get_param()"<<std::endl;

    size_t ret = lc_data.compare(pos, param_length, param);
    if(ret == 0) {
        pos += param_length; //skip over param

        //skip over whitespace until param value is found
        while((lc_data.compare(pos, 1, " ") == 0)&&(pos < eol))
            ++pos;
        
        while((lc_data.compare(eol, 1, " ") == 0)&&(eol > pos))
            --eol;
        return true;
    }

    std::cout<<"lc_data ["<<lc_data<<"]\ndoes not compare to\nparam ["<<param<<"]"<<std::endl;
    
    return false;
}

//removed all bad_char from string data
void robots_txt::sanitize(std::string& data, std::string bad_char)
{
    for(unsigned int i = 0; i < bad_char.length(); ++i)
        data.erase(std::remove(data.begin(), data.end(), bad_char[i]), data.end());
}

//should be called after a 'User-agent:' field has been matched and identified
// pos and eol determine a line (ending in '\n') within &data
// returns position of last instruction within User-agent block
process_state robots_txt::process_instruction(std::string& data, std::string& lc_data, size_t pos, size_t eol)
{
    process_state ret = OK;
    std::cout<<"robots_txt::process_instruction data ["<<data<<"] lc_data ["<<lc_data<<"]"<<std::endl;

    //match param field
    if(get_param(lc_data, pos, eol, "user-agent:")) {
        if(data.compare(pos, 1, "*") == 0) {
            std::cout<<"robots_txt::process_instruction * AGENT_MATCH *"<<agent_name<<std::endl;
            ret = AGENT_MATCH;
        
        } else if(agent_name.size() < eol-pos) {
            if (data.compare(pos, agent_name.size(), agent_name) == 0) {
                std::cout<<"robots_txt::process_instruction  AGENT_MATCH"<<agent_name<<std::endl;
                ret = AGENT_MATCH;
            }
        } else {
            ret = AGENT_MISMATCH;
        }
    }

    else if(process_param) { //found matching agent
        if(get_param(lc_data, pos, eol, "dissalow:")) {
            std::string value = data.substr(pos, eol-pos);

            //wildcard handling
            if((value == "/")||(value == "*")) {
                can_crawl = false;
            } else {
                sanitize(value, "*"); //workaround: remove astrix charecter as I can /NOT/ regex
                exclusion_list.push_back(value);
            }
        } else if(get_param(lc_data, pos, eol, "crawl-delay:")) {
            int int_value;
            std::string value = data.substr(pos, eol-pos);
            std::stringstream str(value);

            str >> int_value;
            if(!str)
                crawl_delay_time = DEFAULT_CRAWL_DELAY;
            else
                crawl_delay_time = int_value;
        } else {
            ret = FAIL;
        }
    }

    return ret;
}

void robots_txt::parse(std::string& data)
{
    size_t data_size = data.size();

    //site does not have a robots.txt or failed to retrieve one
    if(data_size == 0) {
        // set defaults
        can_crawl = true;
        crawl_delay_time = DEFAULT_CRAWL_DELAY;
    } else if(data_size > MAX_DATA_SIZE) {
        std::cerr<<"data_size > MAX_DATA_SIZE"<<std::endl;
    } else {
        std::stringstream stream(data);
        std::string line;

        while(std::getline(stream, line)) {
            if(!line_is_comment(line)) {
                size_t pos = 0;
                size_t eol = line.length();
                
                //find offset to first bit of non whitespace char
                //this should never reach eol due to line_is_comment call
                while((data.compare(pos, 1, " ") == 0)&&(pos < eol))
                    ++pos;

                //edge case, should never happen as line is not a comment
                if(pos > eol) {
                    std::cerr<<"robots_txt::parse pos > eol (line.length())"<<std::endl;
                    break;
                }

                //create an all lowercase line for param field matching
                std::string lc_line;
                std::transform(line.begin(), line.end(), lc_line.begin(), lower_case);

                process_state res = process_instruction(line, lc_line, pos, eol);

                switch(res) {
                case AGENT_MATCH:
                    process_param = true;
                    break;
                case AGENT_MISMATCH:
                    process_param = false;
                    break;
                case OK:
                    break;
                case FAIL:
                    std::cerr<<"robots_txt::parse() process_instruction returned FAIL"<<std::endl;
                    exit(-1);
                    break;
                }
            }
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




