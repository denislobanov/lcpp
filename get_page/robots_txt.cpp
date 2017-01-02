#include <iostream>
#include <sstream>
#include <ctime>
#include <vector>
#include <algorithm>

#include "robots_txt.hpp"
#include "netio.hpp"

//Local defines
#if (defined(DEBUG))&&(DEBUG > 2)
#include <fstream>
#endif

#if defined(DEBUG)
    #define dbg std::cout<<__FILE__<<"("<<__LINE__<<"): "
    #if DEBUG > 1
        #define dbg_1 std::cout<<__FILE__<<"("<<__LINE__<<"): "
    #else
        #define dbg_1 0 && std::cout
    #endif
#else
    #define dbg 0 && std::cout
    #define dbg_1 0 && std::cout
#endif

robots_txt::robots_txt(std::string user_agent, std::string root_domain)
{
    agent_name = user_agent;
    domain = root_domain;
    process_param = false;  //set to true on matching user-agent by process instruction
    last_visit = 0;
    crawl_delay = DEFAULT_CRAWL_DELAY;
}

robots_txt::~robots_txt(void)
{
    //?
}

void robots_txt::fetch(netio& netio_obj)
{
    //(re)set defaults
    disallow_list.clear();
    allow_list.clear();
    can_crawl = true;
    crawl_delay = DEFAULT_CRAWL_DELAY;

#if (defined(DEBUG))&&(DEBUG > 2)
    //for debug, use file instead
    std::fstream debug_file;
    debug_file.open("robots.txt");
    std::string temp_data((std::istreambuf_iterator<char>(debug_file)),
                           std::istreambuf_iterator<char>());
#else
    std::string temp_data;
    netio_obj.fetch(&temp_data, domain+"/robots.txt");
#endif


    parse(temp_data);
}

bool robots_txt::exclude(std::string& path)
{
    if(!can_crawl)
        return true;

    size_t pos = domain.length();

    for(std::vector<std::string>::iterator it = disallow_list.begin(); it != disallow_list.end(); ++it) {
        if(path.compare(pos, it->size(), *it) == 0)
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
            if(data.empty())
                return true;
            else if(data.compare(pos, 1, "#") == 0)
                return true;

            //should this be removed?
            else if(data.compare(pos, 1, " ") == 0)
                return false;
        } while((data.compare(++pos, 1, " ") == 0)&&(pos < eol));
    }

    return false;
}

//matches lower case param representation in lc_data, if found pos and eol
//are modified to represent the data after the param without whitespace
//  and returns true
//else returns false, pos and eol unmodified
bool robots_txt::get_param(std::string& lc_data, size_t& pos, size_t& eol, std::string param)
{
    size_t param_length = param.length();

    dbg<<"robots_txt::get_param()"<<std::endl;

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

    dbg<<"\t lc_data ["<<lc_data<<"]\n  does not compare to\n\tparam ["<<param<<"]"<<std::endl;

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
void robots_txt::process_instruction(std::string& data, std::string& lc_data, size_t pos, size_t eol)
{
    //match param field
    if(get_param(lc_data, pos, eol, "user-agent:")) {
        if(data.compare(pos, 1, "*") == 0) {
            dbg<<"robots_txt::process_instruction * AGENT_MATCH *"<<std::endl;
            process_param = true;

        } else if(agent_name.size() < eol-pos) {
            if (data.compare(pos, agent_name.size(), agent_name) == 0) {
                dbg<<"robots_txt::process_instruction  AGENT_MATCH "<<agent_name<<std::endl;
                process_param = true;
            } else {
                dbg_1<<"AGENT_UNMATCH"<<std::endl;
                process_param = false;
            }

        } else {
            std::cerr<<"agent_name.size() > eol-pos"<<std::endl;
            std::cerr<<"string was ["<<lc_data<<"]"<<std::endl;
            process_param = false;
        }
    }

    //sitemap directive has file (instead of user-agent) scope
    else if(get_param(lc_data, pos, eol, "sitemap:")) {
        std::string value = data.substr(pos, eol-pos);

        sitemap_url = value;
        dbg<<"sitemap: ["<<value<<"]"<<std::endl;
    }

    else if(process_param) { //found matching agent
        if(get_param(lc_data, pos, eol, "disallow:")) {
            std::string value = data.substr(pos, eol-pos);

            //wildcard handling
            if((value == "/")||(value == "*")) {
                can_crawl = false;
            } else {
                sanitize(value, "*"); //workaround: remove astrix charecter as I can /NOT/ regex
                disallow_list.push_back(value);
            }
        } else if(get_param(lc_data, pos, eol, "crawl-delay:")) {
            int int_value;
            std::string value = data.substr(pos, eol-pos);
            std::stringstream str(value);

            str >> int_value;
            if(str)
                crawl_delay = int_value;
        } else if(get_param(lc_data, pos, eol, "allow:")) {
            std::string value = data.substr(pos, eol-pos);

            if(value == "/") {
                can_crawl = true;
            } else if(value == "*") {
                can_crawl = true;
                value = "/";    //same effect, '*' are sanitized
            }

            //push_back always to prune disallow_list
            sanitize(value, "*");
            allow_list.push_back(value);

            dbg_1<<"robots_txt::process_instruction found allow value ["<<value<<"]"<<std::endl;
        }
    }
}

void robots_txt::parse(std::string& data)
{
    size_t data_size = data.size();

    //site does not have a robots.txt or failed to retrieve one
    if(data_size == 0) {
        // reset defaults
        can_crawl = true;
        crawl_delay = DEFAULT_CRAWL_DELAY;
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
                std::string lc_line = line;
                std::transform(lc_line.begin(), lc_line.end(), lc_line.begin(), ::tolower);

                dbg_1<<"line ["<<line<<"] lc_line ["<<lc_line<<"]"<<std::endl;

                process_instruction(line, lc_line, pos, eol);
            }
        }

        //prune disallow_list
        if(allow_list.size() > 0) {
            dbg_1<<"--------\tpruning disallow_list\t--------"<<std::endl;

            disallow_list.erase(std::remove_if(disallow_list.begin(), disallow_list.end(),
                    [&, this](std::string& s) -> bool {
                        //iterate through allow_list vector, return true if s matches.
                        for(std::vector<std::string>::iterator it = allow_list.begin(); it != allow_list.end(); ++it)
                            if(s.compare(0, it->size(), *it) == 0)
                                return true;

                        return false;
                    }),
                disallow_list.end()
            );
        }
    }
}

void robots_txt::import_exclusions(std::vector<std::string>& data)
{
    disallow_list = data;
}

bool robots_txt::export_exclusions(std::vector<std::string>& data)
{
    data = disallow_list;

    return disallow_list.size() > 0;
}

void robots_txt::import_inclusions(std::vector<std::string>& data)
{
    allow_list = data;
}

bool robots_txt::export_inclusions(std::vector<std::string>& data)
{
    data = allow_list;

    return allow_list.size() > 0;
}

bool robots_txt::sitemap(std::string& data)
{
    if (sitemap_url.size() > 0) {
        data = sitemap_url;
        return true;
    }

    return false;
}
