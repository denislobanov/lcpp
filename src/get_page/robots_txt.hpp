#if !defined (ROBOTS_TXT_H)
#define ROBOTS_TXT_H

#include <iostream>
#include <ctime>
#include <vector>

class netio;

/**
 * if the time between visits is greater than this, page should be requeued
 * without crawling
 */
#define REVISIT_TOO_LONG    1000

/**
 * in the venet that robots.txt does not provide a Crawl-delay: command, use this
 */
#define DEFAULT_CRAWL_DELAY 1

class robots_txt
{
    public:
    /**
     * creates a robots_txt parser instance.
     *
     * netio used only during object creation and by optionall parser() call
     * crawler_name is needed to check "User-agent:" field of robots.txt
     * root_domain will have robots.txt appended automatically
     */
    robots_txt(netio& netio_obj, std::string user_agent, std::string root_domain);
    ~robots_txt(void);

    /**
     * optional call to refresh current robots.txt profile
     */
    void refresh(netio& netio_obj);

    /**
     * checks if path (usually url) is within the exclusion list
     * (robots.txt "Dissalow: ")
     */
    bool excluded_dir(std::string& path);
    
    /**
     * how long to wait before recrawls
     */
    time_t crawl_delay(void);

    /**
     * to avoid thrashing netio for robots.txt (cache_time < robots.txt expire time)
     * performs a copy operation
     */
    void import_exclusions(std::vector<std::string>& data);
    void export_exclusions(std::vector<std::string>& data);

    private:
    bool can_crawl; //if crawler's completely banned or a whitelist policy is used
    
    std::string agent_name;
    std::string domain;
    std::vector<std::string> exclusion_list;
    time_t crawl_delay_time;

    void parse(std::string& data);
    size_t line_is_comment(std::string& data, size_t pos);
    size_t line_length(std::string& data, size_t pos);
    bool match_agent(std::string& data, size_t pos, size_t eol);
    std::string get_param(std::string& data, size_t pos, size_t eol, std::string param, std::string deliminator);
    size_t process_instruction(std::string& data, size_t pos, size_t eol);
};

#endif
