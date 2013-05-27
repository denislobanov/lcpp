#include <iostream>

#include "page_data.hpp"

class cache;
class database;

/**
 * controls allocating/deletion of page_data_s'
 *      for now only uses new/delete
 * 
 * to do/
 *      use a pool of pre allocated page_data_s
 *          -- fifo?
 *      get/put to from pool
 */
class memory_mgr
{
    public:
    memory_mgr(std::string database_path);
    ~memory_mgr(void);

    struct page_data_s* get_page(void);
    void put_page(std::string& url, struct page_data_s* page);

    private:
    cache* mem_cache;
    database* mem_db; 
};
