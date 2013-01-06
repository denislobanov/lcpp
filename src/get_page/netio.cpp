#include <iostream>
#include <mutex>
#include <curl/curl.h>

#include "netio.h"

netio::netio(std::string user_agent_string)
{
    user_agent = user_agent_string;
    
    //initialise libCURL
    lib_handle = curl_easy_init();
}

netio::~netio(void)
{
    curl_easy_cleanup(lib_handle);
}

bool netio::fetch(std::string url)
{
    //locking is needed as lib_handle cannot be used simmultaniously
    //both setting url and retrieving data must be locked for data to
    //coincide with url
    lib_mutex.lock();
    curl_easy_setopt(lib_handle, CURLOPT_URL, url.c_str());
    curl_ret = curl_easy_perform(lib_handle);
    lib_mutex.unlock();

    return (curl_ret == CURLE_OK)?true:false;
}

std::string netio::last_error(void)
{
    return error_buffer;
}

//this function should only be called once and before anything else.
//no locking for now for simplicity
bool netio::default_config(bool debug)
{
    curl_ret = curl_easy_setopt(lib_handle, CURLOPT_VERBOSE, ((debug == true)?1:0));
    if(curl_ret != CURLE_OK)
        return false;

    //options
    if((curl_ret = curl_easy_setopt(lib_handle, CURLOPT_HEADER, 1)) != CURLE_OK)
        return false;
    if((curl_ret = curl_easy_setopt(lib_handle, CURLOPT_NOPROGRESS, 1)) != CURLE_OK)
        return false;
    if((curl_ret = curl_easy_setopt(lib_handle, CURLOPT_NOSIGNAL, 1)) != CURLE_OK)
        return false;
    if((curl_ret = curl_easy_setopt(lib_handle, CURLOPT_USERAGENT, user_agent.c_str())) != CURLE_OK)
        return false;

    return true;
}

void netio::reset_config(void)
{
    curl_easy_reset(lib_handle);
}
