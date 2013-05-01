#include <iostream>
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
size_t robots_txt::line_is_comment(std::string& data, size_t pos)
{
    if(pos > 0)
        if(data.compare(--pos, 1, "#") == 0)
            return true;

    return false;
}

//finds end-of-line (eol,  '\n')
//does not consider whitespace, for user agent fields..
size_t robots_txt::line_end(std::string& data, size_t pos)
{
    return data.find("\n", pos);
}

bool robots_txt::match_agent(std::string& data, size_t pos, size_t eol)
{
    //ignore first whitespace after 'User-agent:'
    if(data.compare(++pos, 1, "*") == 0) {
        std::cout<<"robots_txt::match_agent *"<<std::endl;
        return true;
    }

    size_t res = data.compare(pos, eol-pos, agent_name); //eol-pos = line length
    if((res == 0)||(res == agent_name.size())) {
        std::cout<<"robots_txt::match_agent "<<agent_name<<std::endl;
        return true;
    } else {
        std::cout<<"robots_txt::match_agent no match"<<std::endl;
        return false;
    }
}

//returns substring of param, modifies eol (based on deliminator)
std::string robots_txt::get_param(std::string& data, size_t pos, size_t eol, std::string param, std::string deliminator)
{
    size_t param_length = param.size();

    std::cout<<"robots_txt::get_param pos = "<<pos<<" eol = "<<eol<<std::endl;

    size_t ret = data.compare(pos, eol-pos, param);
    if((ret == 0)||(ret == param_length)) {
        //generate new eol, as urls cant have whiletspace
        eol = data.find_first_of(deliminator, pos) - pos;
        return data.substr(pos+param_length, eol);
    }

    return 0;
}

//removed all bad_char from string data
void robots_txt::sanitize(std::string& data, std::string bad_char)
{
    for(unsigned int i = 0; i < bad_char.length(); ++i)
        data.erase(std::remove(data.begin(), data.end(), bad_char[i]), data.end());
}

//should be called after a 'User-agent:' field has been matched and identified
//returns position of last instruction within User-agent block
size_t robots_txt::process_instruction(std::string& data, size_t pos, size_t eol)
{
    size_t last_good_pos = pos;

    std::cout<<"robots_txt::process_instruction pos = "<<pos<<" eol = "<<eol<<std::endl;
    while(pos < data.size()) {
        if(!line_is_comment(data, pos)) {
            std::string res;

            if((res = get_param(data, pos, eol, "Disallow:", " \t\n")).size() > 0) {
                if((res == "/")||(res == "*")) {
                    can_crawl = false;
                } else {
                    //workaround: remove astrix charecter as I cant regex
                    sanitize(data, "*");
                    exclusion_list.push_back(res);
                }
                last_good_pos = pos;

            } else if((res = get_param(data, pos, eol, "Crawl-delay:", " \t\n")).size() > 0) {
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
        }

        //got to next line
        pos = ++eol;
        eol = line_end(data, pos);
    }

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
    } else {
        std::cout<<"robots_txt::parse::got data"<<std::endl;
        // parse
        std::string user_agent_field = "User-agent:";
        size_t user_agent_size = user_agent_field.size();
        size_t pos = 0;

        while(pos < data_size) {
            std::cout<<"robots_txt::parse::while(pos < data_size) pos = "<<pos<<std::endl;
            pos = data.find(user_agent_field, pos);

            //found something
            if(pos != std::string::npos) {
                if(!line_is_comment(data, pos)) {
                    std::cout<<"robots_txt::parse::data valid, pos = "<<pos<<std::endl;
                    //tokenize by '\n'
                    size_t eol = line_end(data, pos);
                    std::cout<<"robots_txt::parse::line_end pos = "<<pos<<" eol = "<<eol<<std::endl;

                    //applicable to us?
                    if(match_agent(data, pos+user_agent_size, eol)) {
                        std::cout<<"robots_txt::parse::user agent match, pos = "<<pos<<" eol = "<<eol<<std::endl;
                        pos = eol + 1;  //skip over "User-agent:" line
                        pos = process_instruction(data, pos, eol);
                    }
                }
                ++pos;
            } else {
                //robots.txt exists but doesnt apply to us
                //set defaults and quit
                can_crawl = true;
                crawl_delay_time = DEFAULT_CRAWL_DELAY;
                break;
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




