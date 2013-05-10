#include <iostream>
#include <sstream>  //stringstream for line orientated parsing
#include <ctime>
#include <sstream>  //convert int to string
#include <vector>
#include <algorithm>    //std::remove, to remove '*' from exlusion_list

#include "robots_txt.hpp"
#include "netio.h"

robots_txt::robots_txt(netio& netio_obj, std::string user_agent, std::string root_domain)
{
    agent_name = user_agent;
    domain = root_domain;

    refresh(netio_obj);
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

    if(data.size > 0) {
        do {
            if(data[pos] == "#")
                return true;
            else if(data[pos] == "\n")
                return true;
            else if(data[pos] != " ")
                return false;
        } while((data[++pos] == " ")&&(pos < data.size()));
    }

    return false;
}

//finds end-of-line (eol,  '\n')
//does not consider whitespace, for user agent fields..
size_t robots_txt::line_end(std::string& data, size_t pos)
{
    return data.find("\n", pos);
}


bool robots_txt::match_agent(std::string& data)
{
    size_t pos = 0;
    size_t eol = data.size();

    //ignore leading whitespace, this should never reach eol due to line_is_comment call
    while(data[pos] == " ")
        ++pos;

    //edge case, should never happen as line is not a comment
    if(pos >= eol) {
        std::cerr<<"robots_txt::match_agent pos >= data.size()"<<std:endl;
        return false;
    }

    lowercase_data = data;
    lowercase_data.tolower();
    
    if((pos = lowercase_data.find("user-agent:", pos)) != std::npos) {
        //found User-agent field
        while((data[pos] == " ")&&(pos < eol))
            ++pos;

        if(pos == eol)
            return false;
        
        //need case sensitive matches after param match
        if(data.compare(pos, 1, "*") == 0) {
            std::cout<<"robots_txt::match_agent *"<<std::endl;
            return true;
        }

        //enought of line left for agent name field
        if(agent_name.size() < eol-pos) {
            if (data.compare(pos, agent_name.size(), agent_name) == 0) {
                std::cout<<"robots_txt::match_agent "<<agent_name<<std::endl;
                return true;
            }
        }
    }

    return false;
}

//searches &data within [pos, eol] constraints for param match
// if match found returns true, &result is set to parameter value
// else returns false
bool robots_txt::get_param(std::string& data, size_t pos, size_t eol, std::string param, std::string& result)
{
    size_t param_length = param.length();

    //FIXME: debug
    std::cout<<"robots_txt::get_param pos = "<<pos<<" eol = "<<eol<<" pos+param_length = "<<pos+param_length<<std::endl;
    std::cout<<"robots_txt::get_param data ["<<data.substr(pos, eol-pos)<<"]"<<" param ["<<param<<"]"<<std::endl;

    size_t ret = data.compare(pos, param_length, param);
    if(ret == 0) {
        result = data.substr(pos+param_length, eol-(pos+param_length));
        std::cout<<"robots_txt::get_param result ["<<result<<"]"<<std::endl;
        return true;
    }

    //FIXME: debug
    std::cout<<"robots_txt::get_param returning false"<<std::endl;
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
size_t robots_txt::process_instruction(std::string& data)
{
    size_t last_good_pos = pos;

    std::cout<<"robots_txt::process_instruction pos = "<<pos<<" eol = "<<eol<<std::endl;
    while(pos < data.length()) {
        if(!line_is_comment(data, pos)) {
            std::string res;

            if(get_param(data, pos, eol, "Disallow:", res)) {
                //strip whitespace
                sanitize(res, " ");
                std::cout<<"res ["<<res<<"]"<<std::endl;

                //shortcut complete bans
                if((res == "/")||(res == "*")) {
                    can_crawl = false;
                } else {
                    //workaround: remove astrix charecter as I can /NOT/ regex
                    sanitize(data, "*");
                    exclusion_list.push_back(res);
                }
                last_good_pos = pos;

            } else if(get_param(data, pos, eol, "Crawl-delay:", res)) {
                int res_int;
                std::stringstream str(res);

                str >> res_int;
                if(!str)
                    crawl_delay_time = DEFAULT_CRAWL_DELAY;
                else
                    crawl_delay_time = res_int;

                last_good_pos = pos;

            } else {
                break;
            }
            std::cout<<"~~~"<<std::endl;
        } else {
            std::cout<<"line is a comment!"<<std::endl;
        }

        //got to next line
        pos = eol;
        eol = line_end(data, pos+1);
    }

    std::cout<<"pos = "<<pos<<" data.length = "<<data.length()<<std::endl;

    return last_good_pos;
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
        std::cout<<"robots_txt::parse::got data"<<std::endl;

        std::stringstream stream(data);
        std::string user_agent_field = "User-agent:";
        std::string line;
        size_t user_agent_size = user_agent_field.size();

        while(std::getline(stream, line)) {
            if(!line_is_comment(line)) {
                
                if(match_agent(line)) {
                    
                    //process instruction should hange User-agent fields too.
                    //compares must be case insensitive!
                    process_instruction





                
                pos = data.find(user_agent_field, pos);

                //found something
                if(pos != std::string::npos) {
                    //tokenize by '\n'
                    size_t eol = line_end(data, pos);
                    std::cout<<"robots_txt::parse::line_end pos = "<<pos<<" eol = "<<eol<<" ["<<data.substr(pos, eol-pos)<<"]"<<std::endl;

                    //applicable to us?
                    if(match_agent(data, pos+user_agent_size, eol)) {
                        //FIXME: debug
                        std::cout<<"robots_txt::parse pos = "<<pos<<" eol = "<<eol<<std::endl;

                        pos = ++eol;  //skip over "User-agent:" line
                        eol = line_end(data, pos);
                        pos = process_instruction(data, pos, eol);
                    }
                    ++pos;

                } else {
                    std::cout<<"pos = std::string::npos"<<std::endl;
                    //robots.txt exists but doesnt apply to us
                    //set defaults and quit
                    can_crawl = true;
                    crawl_delay_time = DEFAULT_CRAWL_DELAY;
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




