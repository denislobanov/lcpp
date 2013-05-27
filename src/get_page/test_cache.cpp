#include <iostream>
#include <vector>
#include <sstream>

#include "page_data.hpp"
#include "cache.hpp"

int main(void)
{
    cache test_cache;
    std::ostringstream oss;
    int i;
    
    struct page_data_s *test_page;
    std::vector<struct page_data_s*> memory_pool;

    //test ctl
    const int max_pages = 2*PC_UPPER_WATERMARK;
    const bool single_step = true;
    
    std::cout<<"filling cache with "<<max_pages<<" entires"<<std::endl;
    for(i = 0; i < max_pages; ++i) {
        //generate test url
        oss.str("");
        oss<<"http://test_url_"<<i<<".com/test_page"<<i<<".html";
        std::string test_url = oss.str();
        
        //create a blank page
        test_page = new struct page_data_s;
        memory_pool.push_back(test_page);

        //fill with test data
        test_page->rank = i;
        test_page->url = test_url;
        oss.str("");
        oss<<"test_cache.cpp generated page "<<i;
        test_page->description = oss.str();

        //send to cache
        test_cache.put_page_data(test_page, test_url);
        test_page->access_lock.unlock();    //normally done by memory_mgr class

        std::cout<<"added page "<<i<<" to cache"<<std::endl;
        std::cout<<"page rank:"<<test_page->rank<<std::endl;
        std::cout<<"page desc:"<<test_page->description<<std::endl;
    }

    if(single_step) {
        std::cout<<"\n~~~\ngetting "<<i<<" pages"<<std::endl;
        for(i = 0; i < max_pages; ++i) {
            //generate test url
            oss.str("");
            oss<<"http://test_url_"<<i<<".com/test_page"<<i<<".html";
            std::string test_url = oss.str();

            //retrieve test page
            test_page = new struct page_data_s;
            if(test_cache.get_page_data(&test_page, test_url)) {
                std::cout<<"page "<<i<<" rank "<<test_page->rank<<std::endl;
                std::cout<<"page "<<i<<" description ["<<test_page->description<<"]\n"<<std::endl;
                
            } else {
                std::cout<<"page "<<i<<" not in cache\n"<<std::endl;
                delete test_page;
            }
        }
    }

    //delete test pages
    std::cout<<"freeing memory"<<std::endl;
    //~ for(auto page: memory_pool)
        //~ delete page;

    return 0;
}
