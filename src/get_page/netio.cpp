#include <iostream>
#include <mutex>
#include <vector>
#include <curl/curl.h>

#include "netio.h"

netio::netio(std::string user_agent_string)
{
    user_agent = user_agent_string;
    header_data.reserve(CURL_MAX_HTTP_HEADER);
    
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

    //callbacks
    //~ if((curl_ret = curl_easy_setopt(lib_handle, CURLOPT_HEADERFUNCTION, &netio::store_header)) != CURLE_OK)
        //~ return false;
    if((curl_ret = curl_easy_setopt(lib_handle, CURLOPT_WRITEFUNCTION, &netio::store_data)) != CURLE_OK)
        return false;
    if((curl_ret = curl_easy_setopt(lib_handle, CURLOPT_WRITEDATA, &page_data)) != CURLE_OK)
        return false;

    return true;
}

void netio::reset_config(void)
{
    curl_easy_reset(lib_handle);
}

size_t netio::store_header(char *ptr, size_t size, size_t nmemb, void *userp)
{
    std::cout<<"store_header"<<std::endl;
    return 0;
}

size_t netio::store_data(char *ptr, size_t size, size_t nmemb, void *userp)
{
    std::cout<<"store_data"<<std::endl;
    size_t real_size = size*nmemb;
    std::vector<char>* real_data = (std::vector<char> *)userp;
    
    const char* start = ptr;
    const char* end = start+real_size;
    std::copy(start, end, std::back_inserter(*real_data));
    
    return real_size;
}
