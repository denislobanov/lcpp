#ifndef NETIO_H
#define NETIO_H

#include <iostream>
#include <mutex>
#include <curl/curl.h>

class netio
{
    public:
    netio(std::string user_agent_string);
    ~netio();
    bool fetch(std::string url);
    std::string last_error(void);
    bool default_config(bool debug);
    void reset_config(void);

    private:
    std::mutex lib_mutex;
    CURL* lib_handle;
    CURLcode curl_ret;

    std::string user_agent;
    std::string error_buffer;
};

#endif
