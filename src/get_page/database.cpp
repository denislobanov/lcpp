#include <iostream>

#include "database.hpp"
#include "page_data.hpp"

database::database(std::string path)
{
    db_path = path;
}

database::~database(void)
{

}

struct page_data_s* database::get_page(std::string& url)
{
    return 0;
}

void database::put_page(std::string& url, struct page_data_s* page_data)
{

}
