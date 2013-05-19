#if !defined(DATABASE_H)
#define DATABASE_H

#include <iostream>
#include "page_data.hpp"

class database
{
    public:
    database(std::string path);
    ~database(void);

    /**
     * url is hashed and a page_data_s retrieved.
     * if no page_data_s is present, a new one is allocated.
     */
    struct page_data_s* get_page(std::string& url);

    /**
     * page is potentially cached, depending on criteria and sent to the db
     */
    void put_page(std::string& url, struct page_data_s* page_data);

    private:
    std::string db_path;
};

#endif
