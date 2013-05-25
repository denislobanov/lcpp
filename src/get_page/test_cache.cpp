#include <iostream>
#include <vector>
#include <sstream>

#include "page_data.hpp"
#include "cache.hpp"
#include "database.hpp"

int main(void)
{
    database my_database("./test_db");
    cache test_cache(&my_database);
    std::ostringstream oss;
    int i;
    
    page_data_s *test_page; //allocated and deleted by cache

    //test ctl
    const int max_pages = 2*PC_UPPER_WATERMARK;   //no db access
    //~ const int single_read_runs = 10;
    //~ const int single_rw_runs = 20;
    //~ const int multi_r_runs  = 20;
    //~ const int multi_rw_runs = 40;
    const bool single_step = true;
    
    std::cout<<"filling cache with "<<max_pages<<" entires"<<std::endl;
    for(i = 0; i < max_pages; ++i) {
        //generate test url
        oss.str("");
        oss<<"http://test_url_"<<i<<".com/test_page"<<i<<".html";
        std::string test_url = oss.str();
        
        //get blank page
        test_page = test_cache.get_page(test_url);

        if(test_page == nullptr)
            std::cerr<<"test_page ["<<i<<"] == nullptr"<<std::endl;

        //fill with test data
        test_page->rank = i;
        test_page->url = test_url;
        oss.str("");
        oss<<"test_cache.cpp generated page "<<i;
        test_page->description = oss.str();

        //send to cache
        test_cache.put_page(test_url, test_page);
        test_page = nullptr;
        test_page = test_cache.get_page(test_url);

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
            test_page = test_cache.get_page(test_url);

            //~ std::cout<<test_page->description<<std::endl;
            std::cout<<"page "<<i<<" rank "<<test_page->rank<<std::endl;
            std::cout<<"page "<<i<<" description ["<<test_page->description<<"]"<<std::endl;
        }

        
    } else {
        std::cerr<<"todo"<<std::endl;
    }

    return 0;
}
