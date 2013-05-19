#include <iostream>
#include <vector>

#include "page_data.hpp"
#include "cache.hpp"
#include "database.hpp"

int main(void)
{
    database my_database("./test_db");
    cache test_cache(&my_database);
    
    page_data_s *test_page; //allocated and deleted by cache

    //test ctl
    const int max_pages = PC_UPPER_WATERMARK;   //no db access
    const int single_read_runs = 10;
    const int single_rw_runs = 20;
    const int multi_r_runs  = 20;
    const int multi_rw_runs = 40;
    const bool single_step = true;
    
    std::cout<<"filling cache with "<<max_pages<<" entires"<<std::endl;
    for(int i = 0; i < max_pages; ++i) {
        //generate test url
        std::string test_url = "http://test_url_"<<i<<".com/test_page"<<i<<".html";
        
        //get blank page
        test_page = test_cache.get_page(test_url);

        //fill with test data
        test_page->rank = i;
        test_page->description = "test_cache.cpp generated page "<<i;

        //send to cache
        test_cache.put_page(test_page, test_url);

        std::cout<<"added page "<<i<<" to cache"<<std::endl;
    }

    if(single_step) {
        std::cout<<"getting "<<i<<" pages"<<std::end;
        for(int i = 0; i < max_pages; ++i) {
            //generate test url
            std::string test_url = "http://test_url_"<<i<<".com/test_page"<<i<<".html";

            //retrieve test page
            test_page = test_cache.get_page(test_url);

            std::cout<<test_page->description<<std::end;
            std::cout<<"page rank "<<test_page->rank<<"\n~~~~"<<std::endl;
        }

        
    } else {
        std::cerr<<"todo"<<std::endl;
    }

    return 0;
}
